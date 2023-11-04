//
//  main.cpp
//  concept
//
//  Created by Kenneth Esdaile on 9/16/23.
//

#include <iostream>

//#define GLFW_INCLUDE_VULKAN
//#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include "cgm.hpp"
#include <set>


kege::vec2 ray_vs_sphere( kege::vec3 rp, kege::vec3 rd, kege::vec3 sc, float sr )
{
    kege::vec3  m = rp - sc;
    float b = kege::dot( m, rd );
    float c = kege::dot( m, m ) - sr * sr;

    //// Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
    if (c > 0.0f && b > 0.0f) return kege::vec2( 1000000 );

    float d = b * b - c;

    // A negative discriminant corresponds to ray missing sphere
    if ( d < 0.0 ) return kege::vec2( 1000000 );

    // Ray now found to intersect sphere, compute smallest t value of intersection
    d = sqrt( d );

    // If near is negative, ray started inside sphere so clamp near to zero
    float near = -b - d;
    float far  = -b + d;

    return kege::vec2( near, far );
}






namespace kege{

    struct QuadtreeChildren;

    struct Quadtree
    {
        kege::QuadtreeChildren* _children;
        kege::fvec3 *_vertices[ 4 ];
        kege::fvec3 _position;
        kege::fvec4 _sphere;

        kege::ivec3 _quad;

        int  _depth;
        int  _face;
    };

    struct QuadtreeChildren
    {
        kege::Quadtree _nodes[ 4 ];
        kege::vec3 _vertices[ 4 ];
    };

    struct QuadtreePlane
    {
        kege::vec3 _vertices[ 4 ];
        kege::Quadtree _quadtree;
        uint32_t _face_index;
    };

    class CubicShpereQuadtree
    {
    public:

       enum { FRONT, BACK, LEFT, RIGHT, ABOVE, BELOW, MAX_FACE_COUNT };

        void init
        (
            Quadtree &quadtree, 
            uint32_t depth,
            double radius,
            const kege::ivec3& quad,
            const kege::vec3& position,
            kege::vec3 *v[4]
        );

        void contruct( uint32_t face_index, const kege::vec3 (&v)[4] );
        float getHeightValue( uint32_t face_index, int x, int y )const;
        kege::vec3 normalize( const kege::vec3& p );
        void split( Quadtree &quadtree );
        void merge( Quadtree &quadtree );

        double getResolution
        (
            Quadtree& quadtree,
            const kege::vec3& eye
        );

        void updatePlane
        (
            const kege::vec3& eye,
            Quadtree& quadtree
        );

        void update
        (
            const kege::vec3& eye
        );

        CubicShpereQuadtree
        (
            double radius,
            double resolution,
            uint32_t depth
        );

    private:

        QuadtreePlane _planes[6];

        kege::mat33 _rotation;
        kege::vec3  _position;

        double _max_resolution;
        double _max_depth;
        double _radius;

        uint32_t _texture_size;
    };



    void CubicShpereQuadtree::init
    (
        Quadtree &quadtree, uint32_t depth, double radius, const kege::ivec3& quad, const kege::vec3& position, kege::vec3 *v[4]
    )
    {
        quadtree._vertices[ 0 ] = v[ 0 ];
        quadtree._vertices[ 1 ] = v[ 1 ];
        quadtree._vertices[ 2 ] = v[ 2 ];
        quadtree._vertices[ 3 ] = v[ 3 ];
        quadtree._position = position;
        quadtree._children = nullptr;
        quadtree._depth = depth;
        quadtree._quad = quad;
    }

    void CubicShpereQuadtree::contruct( uint32_t face_index, const kege::vec3 (&p)[4] )
    {
        QuadtreePlane* plane = &_planes[ face_index ];
        plane->_face_index = face_index;

        plane->_vertices[0] = this->normalize( p[0] ) * ( _radius + getHeightValue( face_index, 0, _texture_size ) );
        plane->_vertices[1] = this->normalize( p[1] ) * ( _radius + getHeightValue( face_index, _texture_size, _texture_size ) );
        plane->_vertices[2] = this->normalize( p[2] ) * ( _radius + getHeightValue( face_index, _texture_size, 0 ) );
        plane->_vertices[3] = this->normalize( p[3] ) * ( _radius + getHeightValue( face_index, 0, 0 ) );

        int depth = 0;
        double radius = _radius;
        kege::ivec3 quad = kege::ivec3( _texture_size * 0.5 );
        kege::fvec3 center = (p[0] + p[1] + p[2] + p[3]) * 0.5;
        center = kege::normalize( center )  * ( _radius + getHeightValue( face_index, quad.x, quad.y ));
        kege::fvec3 *corners[4] = { &plane->_vertices[0], &plane->_vertices[0], &plane->_vertices[0], &plane->_vertices[0] };

        init( plane->_quadtree, depth, radius, quad, center, corners );
    }

    kege::vec3 CubicShpereQuadtree::normalize( const kege::vec3& p )
    {
        vec3 n;
        float xx = p.x * p.x;
        float yy = p.y * p.y;
        float zz = p.z * p.z;
        n.x = p.x * sqrt(1.0 - (yy + zz) / 2.0 + (yy * zz) / 3);
        n.y = p.y * sqrt(1.0 - (zz + xx) / 2.0 + (zz * xx) / 3);
        n.z = p.z * sqrt(1.0 - (xx + yy) / 2.0 + (xx * yy) / 3);
        return n;
    }

    void CubicShpereQuadtree::split( Quadtree &quadtree )
    {
        const float  hsize = quadtree._quad.z * 0.5;
        const int    depth  = quadtree._depth    + 1;
        const double radius = quadtree._sphere.w * 0.5;

        kege::ivec3 quads[4] =
        {
            kege::ivec3( quadtree._quad.x - hsize, quadtree._quad.y - hsize, hsize ),
            kege::ivec3( quadtree._quad.x + hsize, quadtree._quad.y - hsize, hsize ),
            kege::ivec3( quadtree._quad.x - hsize, quadtree._quad.y + hsize, hsize ),
            kege::ivec3( quadtree._quad.x + hsize, quadtree._quad.y + hsize, hsize ),
        };

        /*
         *[p0]- - - -[m0]- - - -[p1]
         * |          |          |
         * |    c0    |    c1    |
         * |          |          |
         *[m3]- - - - - - - - -[m1]
         * |          |          |
         * |    c3    |    c2    |
         * |          |          |
         *[p3]- - - -[m2]- - - -[p2]
         */

        kege::vec3 *p[4] =
        {
            quadtree._vertices[0],
            quadtree._vertices[1],
            quadtree._vertices[2],
            quadtree._vertices[3]
        };


        kege::fvec3 m[4] = 
        {
            (*p[0] + *p[1]) * 0.5,
            (*p[1] + *p[2]) * 0.5,
            (*p[2] + *p[3]) * 0.5,
            (*p[3] + *p[0]) * 0.5
        };

        /*
         * compute child quadtree center positions
         */
        kege::fvec3 centers[4];
        centers[ 0 ] = (*p[0] + m[0] + m[3] + quadtree._position) * 0.25;
        centers[ 1 ] = (*p[1] + m[0] + m[1] + quadtree._position) * 0.25;
        centers[ 2 ] = (*p[2] + m[1] + m[2] + quadtree._position) * 0.25;
        centers[ 3 ] = (*p[3] + m[2] + m[3] + quadtree._position) * 0.25;

        /*
         * move the child quadtree centers into position
         */
        centers[ 0 ] = this->normalize( centers[ 0 ] ) * (_radius + getHeightValue( quadtree._face, quads[0].x,quads[0].y ));
        centers[ 1 ] = this->normalize( centers[ 1 ] ) * (_radius + getHeightValue( quadtree._face, quads[0].x,quads[0].y ));
        centers[ 2 ] = this->normalize( centers[ 2 ] ) * (_radius + getHeightValue( quadtree._face, quads[0].x,quads[0].y ));
        centers[ 3 ] = this->normalize( centers[ 3 ] ) * (_radius + getHeightValue( quadtree._face, quads[0].x,quads[0].y ));

        /*
         * compute the 4 positions that makes up each child quadtree corners
         */
        kege::vec3 *corners[4][4] =
        {
            {p[0], &quadtree._children->_vertices[ 0 ], &quadtree._position, &quadtree._children->_vertices[ 3 ] },
            {p[0], &quadtree._children->_vertices[ 0 ], &quadtree._position, &quadtree._children->_vertices[ 3 ] },
            {p[0], &quadtree._children->_vertices[ 0 ], &quadtree._position, &quadtree._children->_vertices[ 3 ] },
            {p[0], &quadtree._children->_vertices[ 0 ], &quadtree._position, &quadtree._children->_vertices[ 3 ] }
        };

        /*
         * create QuadtreeChildren and set its north, south, east, and west mid-point vertex positions
         */
        quadtree._children = new kege::QuadtreeChildren;
        quadtree._children->_vertices[ 0 ] = this->normalize( m[ 0 ] ) * (_radius + getHeightValue( quadtree._face, quads[0].x,quads[0].y - hsize ));
        quadtree._children->_vertices[ 1 ] = this->normalize( m[ 1 ] ) * (_radius + getHeightValue( quadtree._face, quads[0].x + hsize,quads[0].y ));
        quadtree._children->_vertices[ 2 ] = this->normalize( m[ 2 ] ) * (_radius + getHeightValue( quadtree._face, quads[0].x,quads[0].y + hsize ));
        quadtree._children->_vertices[ 3 ] = this->normalize( m[ 3 ] ) * (_radius + getHeightValue( quadtree._face, quads[0].x - hsize,quads[0].y ));

        /*
         * initialize child quadtree parameters
         */
        init( quadtree._children->_nodes[ 0 ], depth, radius, quads[ 0 ], centers[ 0 ], corners[ 0 ]);
        init( quadtree._children->_nodes[ 1 ], depth, radius, quads[ 1 ], centers[ 1 ], corners[ 1 ]);
        init( quadtree._children->_nodes[ 2 ], depth, radius, quads[ 2 ], centers[ 2 ], corners[ 2 ]);
        init( quadtree._children->_nodes[ 3 ], depth, radius, quads[ 3 ], centers[ 3 ], corners[ 3 ]);
    }


    void CubicShpereQuadtree::merge( Quadtree &quadtree )
    {
        if ( quadtree._children )
        {
            merge( quadtree._children->_nodes[ 0 ] );
            merge( quadtree._children->_nodes[ 1 ] );
            merge( quadtree._children->_nodes[ 2 ] );
            merge( quadtree._children->_nodes[ 3 ] );

            delete quadtree._children;
            quadtree._children = nullptr;
        }
    }

    double CubicShpereQuadtree::getResolution
    (
        Quadtree& quadtree,
        const kege::vec3& eye
    )
    {
        double radius_sq = kege::sq( quadtree._sphere.w + quadtree._sphere.w );
        double dist = kege::distsq( quadtree._sphere.xyz(), eye );
        return dist / radius_sq;
    }

    void CubicShpereQuadtree::updatePlane
    (
        const kege::vec3& eye,
        Quadtree& quadtree
    )
    {
        quadtree._sphere = kege::vec4(_position + _rotation * quadtree._position, quadtree._sphere.w);

        double resolution = getResolution( quadtree, eye );

        if( resolution < _max_resolution )
        {
            if ( !quadtree._children && quadtree._depth < _max_depth )
            {
                split( quadtree );
            }

            if( quadtree._children )
            {
                updatePlane( eye, quadtree._children->_nodes[0] );
                updatePlane( eye, quadtree._children->_nodes[1] );
                updatePlane( eye, quadtree._children->_nodes[2] );
                updatePlane( eye, quadtree._children->_nodes[3] );
            }
        }
        else if ( quadtree._children )
        {
            merge( quadtree );
        }
    }

    void CubicShpereQuadtree::update(const kege::vec3 &eye)
    {
        for ( uint i = 0; i < MAX_FACE_COUNT; i++ )
        {
            updatePlane( eye, _planes[ i ]._quadtree );
        }
    }

    CubicShpereQuadtree::CubicShpereQuadtree
    (
        double radius,
        double resolution,
        uint32_t depth
    )
    :   _position(0.0)
    ,   _rotation( 1.0 )
    ,   _max_resolution( resolution )
    ,   _radius( radius )
    ,   _max_depth( depth )
    ,   _texture_size( 1024 )
    {
        kege::vec3 p[ 8 ];
        float length = 1.0f;
        p[0] = kege::vec3(  length,  length,  length );
        p[1] = kege::vec3( -length,  length,  length );
        p[2] = kege::vec3( -length, -length,  length );
        p[3] = kege::vec3(  length, -length,  length );

        p[4] = kege::vec3(  length,  length, -length );
        p[5] = kege::vec3( -length,  length, -length );
        p[6] = kege::vec3( -length, -length, -length );
        p[7] = kege::vec3(  length, -length, -length );

        contruct( FRONT, {p[0], p[1], p[2], p[3]} );
        contruct( BACK,  {p[5], p[4], p[7], p[6]} );
        contruct( LEFT,  {p[1], p[5], p[6], p[2]} );
        contruct( RIGHT, {p[4], p[0], p[3], p[7]} );
        contruct( ABOVE, {p[2], p[6], p[7], p[3]} );
        contruct( BELOW, {p[5], p[1], p[0], p[4]} );
    }


    float CubicShpereQuadtree::getHeightValue( uint32_t face_index, int x, int y )const
    {
        return 0;
    }

}



int main() 
{
    using namespace kege;



    std::cout << sizeof( kege::Quadtree ) <<"\n";
    std::cout << ray_vs_sphere( kege::vec3(-5,0,0), kege::vec3(-1,0,0), kege::vec3(0), 10 ) << '\n';

    return EXIT_SUCCESS;
}
