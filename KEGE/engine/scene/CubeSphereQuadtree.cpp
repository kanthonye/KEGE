//
//  CubeSphereQuadtree.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/14/23.
//

#include "CubeSphereQuadtree.hpp"
namespace kege
{
    void CubeSphereQuadtree::build
    (
        CubeSphereQuadtreeNode& quadtree,
        const int& depth,
        const double& radius,
        const kege::fvec4& cube
    )
    {
        quadtree._position = _radius * kege::normalize( cube.xyz() );

        quadtree._children = nullptr;
        quadtree._cube = cube;

        quadtree._sphere.x = quadtree._position.x;
        quadtree._sphere.y = quadtree._position.y;
        quadtree._sphere.z = quadtree._position.z;
        quadtree._sphere.w = radius;

        quadtree._depth = depth;
    }

    void CubeSphereQuadtree::updateWorldPosition
    (
        CubeSphereQuadtreeNode& quadtree
    )
    {
        quadtree._sphere.x = quadtree._position.x + _position.x;
        quadtree._sphere.y = quadtree._position.y + _position.y;
        quadtree._sphere.z = quadtree._position.z + _position.z;
    }

    double CubeSphereQuadtree::getResolution
    (
        CubeSphereQuadtreeNode& quadtree,
        const kege::vec3& eye
    )
    {
        double radius_sq = kege::sq( quadtree._sphere.w + quadtree._sphere.w );
        double dist = kege::distsq( quadtree._sphere.xyz(), eye );
        return dist / radius_sq;
    }

    bool CubeSphereQuadtree::isLeaf
    (
        CubeSphereQuadtreeNode& quadtree
    )   const
    {
        return quadtree._children == nullptr;
    }

    void CubeSphereQuadtree::split
    (
        CubeSphereQuadtreeFace& root, 
        CubeSphereQuadtreeNode& quadtree
    )
    {
        const kege::vec3 center = quadtree._cube.xyz();
        const int    depth  = quadtree._depth    + 1;
        const double size   = quadtree._cube.w   * 0.5;
        const double radius = quadtree._sphere.w * 0.5;

        enum{ SW,SE,NW,NE };
        kege::fvec3 p[4];
        p[ NE ] = center + root._xaxis * size + root._yaxis * size;
        p[ NW ] = center - root._xaxis * size + root._yaxis * size;
        p[ SW ] = center - root._xaxis * size - root._yaxis * size;
        p[ SE ] = center + root._xaxis * size - root._yaxis * size;

        // construct child nodes
        quadtree._children = new CubeSphereQuadtreeChildren;
        build( quadtree._children->_nodes[ NE ], depth, radius, kege::fvec4( p[ NE ], size ) );
        build( quadtree._children->_nodes[ NW ], depth, radius, kege::fvec4( p[ NW ], size ) );
        build( quadtree._children->_nodes[ SE ], depth, radius, kege::fvec4( p[ SE ], size ) );
        build( quadtree._children->_nodes[ SW ], depth, radius, kege::fvec4( p[ SW ], size ) );
    }

    void CubeSphereQuadtree::merge
    (
        CubeSphereQuadtreeNode& quadtree
    )
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

    void CubeSphereQuadtree::update
    (
        const kege::vec3& eye,
        CubeSphereQuadtreeFace& root,
        CubeSphereQuadtreeNode& quadtree
    )
    {
        updateWorldPosition( quadtree );
        double resolution = getResolution( quadtree, eye );

        if( resolution < _max_resolution )
        {
            if ( isLeaf( quadtree ) && quadtree._depth < _max_depth )
            {
                split( root, quadtree );
            }

            if( !isLeaf( quadtree ) )
            {
                update( eye, root, quadtree._children->_nodes[0] );
                update( eye, root, quadtree._children->_nodes[1] );
                update( eye, root, quadtree._children->_nodes[2] );
                update( eye, root, quadtree._children->_nodes[3] );
            }
        }
        else if ( !isLeaf( quadtree ) )
        {
            merge( quadtree );
        }
    }


    void CubeSphereQuadtree::update
    (
        const kege::vec3& eye
    )
    {
        for ( uint i = 0; i < MAX_FACE_COUNT; i++ )
        {
            update( eye, _faces[ i ], _faces[ i ]._quadtree );
        }
    }

    void CubeSphereQuadtree::cullGeometry
    (
        const kege::Frustum<float>& frustum,
        CubeSphereQuadtreeNode& quadtree,
        kege::mat24* instance,
        uint32_t& count
    )
    {
        if ( !isLeaf( quadtree ) )
        {
            cullGeometry( frustum, quadtree._children->_nodes[ 0 ], instance, count );
            cullGeometry( frustum, quadtree._children->_nodes[ 1 ], instance, count );
            cullGeometry( frustum, quadtree._children->_nodes[ 2 ], instance, count );
            cullGeometry( frustum, quadtree._children->_nodes[ 3 ], instance, count );
        }
        else
        {
            kege::vec3 sphere_center = quadtree._sphere.xyz();
            float dp = kege::dot( frustum.GetForwardVector(), kege::normalize( sphere_center ) );
            if ( dp < 0.0 )
            {
                return;
            }

            if ( frustum.TestSphere( sphere_center, quadtree._sphere.w ) )
            {
                if ( count >= _batch_mngr.sizeCap() )
                {
                    _batch_mngr.submitBatch( instance, count );
                    count = 0;
                }
                instance[ count ][ 0 ] = quadtree._cube;
                instance[ count ][ 1 ] = kege::vec4( _radius, 0.0f, 0.0f, 0.0f );
                count++;
            }
        }
    }

    void CubeSphereQuadtree::cullGeometry
    (
        const kege::Frustum<float>& frustum
    )
    {
        uint32_t count = 0;
        std::vector< kege::mat24 > instance( _batch_mngr.sizeCap() );
        _batch_mngr.beginBatchSubmition();
        for ( uint i = 0; i < MAX_FACE_COUNT; i++ )
        {
            cullGeometry( frustum, _faces[ i ]._quadtree, instance.data(), count );
            if ( count != 0 )
            {
                _batch_mngr.submitBatch( instance.data(), count );
                count = 0;
            }
        }
        _batch_mngr.endBatchSubmition();
    }

    void CubeSphereQuadtree::cullGeometry
    (
        CubeSphereQuadtreeNode& quadtree,
        CubeSphereFaceType face_type,
        kege::mat24* instance,
        uint32_t& count
    )
    {
        if( !isLeaf( quadtree ) )
        {
            cullGeometry( quadtree._children->_nodes[ 0 ], face_type, instance, count );
            cullGeometry( quadtree._children->_nodes[ 1 ], face_type, instance, count );
            cullGeometry( quadtree._children->_nodes[ 2 ], face_type, instance, count );
            cullGeometry( quadtree._children->_nodes[ 3 ], face_type, instance, count );
        }
        else
        {
            if ( count >= _batch_mngr.sizeCap() )
            {
                _batch_mngr.submitBatch( instance, count );
                count = 0;
            }
            instance[ count ][ 0 ] = quadtree._cube;
            instance[ count ][ 1 ] = kege::vec4( face_type, _radius, 0.0f, 0.0f );
            count++;
        }
    }

    void CubeSphereQuadtree::cullGeometry()
    {
        uint32_t count = 0;
        std::vector< kege::mat24 > instance( _batch_mngr.sizeCap() );
        _batch_mngr.beginBatchSubmition();
        for ( uint i = 0; i < MAX_FACE_COUNT; i++ )
        {
            cullGeometry( _faces[ i ]._quadtree, _faces[ i ]._face_type, instance.data(), count );
            if ( count != 0 )
            {
                _batch_mngr.submitBatch( instance.data(), count );
                count = 0;
            }
        }

//        CubeSphereFaceType face = CubeSphereFaceType::FRONT;
//        cullGeometry( _faces[ face ]._quadtree, _faces[ face ]._face_type, instance.data(), count );
//        if ( count != 0 )
//        {
//            _batch_mngr.submitBatch( instance.data(), count );
//            count = 0;
//        }

        _batch_mngr.endBatchSubmition();
    }

    void CubeSphereQuadtree::build
    (
        CubeSphereFaceType face_type,
        const kege::mat43& points
    )
    {
        CubeSphereQuadtreeFace* face = &_faces[ face_type ];

        face->_face_type = face_type;

        face->_xaxis = kege::normalize( points[3] - points[2] );
        face->_yaxis = kege::normalize( points[1] - points[2] );
        kege::vec3 n = kege::normalize( kege::cross( face->_xaxis, face->_yaxis ) );


        kege::real  size = 1.f;
        kege::fvec3 center = (((points[0] + points[1] + points[2] + points[3]) * 0.25) - n) + (n * size);
        kege::fvec4 cube( center, size );

        double sphere_radius = _radius + _radius * 0.2928932188; // 0.2928932188 = sin(45.0)

        build( face->_quadtree, 0, sphere_radius, cube );

        //std::cout << face->_xaxis<< "\n";
        //std::cout << face->_yaxis<< "\n";
        //std::cout << "\n";
    }

    const CubeSphereQuadtreeFace* CubeSphereQuadtree::getFace( uint32_t i )const
    {
        return &_faces[ i ];
    }

    const kege::mat34* CubeSphereQuadtree::getFaceRotationMatrices()const
    {
        return _face_rotations;
    }

    void CubeSphereQuadtree::setRotation( const kege::mat33& rotation )
    {
        _rotation = rotation;
    }

    void CubeSphereQuadtree::setPosition( const kege::vec3& position )
    {
        _position = position;
    }

    kege::BatchManager* CubeSphereQuadtree::getBatchBufferManager()
    {
        return &_batch_mngr;
    }

    void CubeSphereQuadtree::buildGeometry()
    {
        kege::vec3 p[ 8 ];
        kege::mat43 vertices[ CubeSphereFaceType::MAX_FACE_COUNT ];

        float length = 1.0f;
        p[0] = kege::vec3(  length,  length,  length );
        p[1] = kege::vec3( -length,  length,  length );
        p[2] = kege::vec3( -length, -length,  length );
        p[3] = kege::vec3(  length, -length,  length );

        p[4] = kege::vec3(  length,  length, -length );
        p[5] = kege::vec3( -length,  length, -length );
        p[6] = kege::vec3( -length, -length, -length );
        p[7] = kege::vec3(  length, -length, -length );

        vertices[ CubeSphereFaceType::FRONT ][0] = p[0];
        vertices[ CubeSphereFaceType::FRONT ][1] = p[1];
        vertices[ CubeSphereFaceType::FRONT ][2] = p[2];
        vertices[ CubeSphereFaceType::FRONT ][3] = p[3];

        vertices[ CubeSphereFaceType::BACK ][0] = p[5];
        vertices[ CubeSphereFaceType::BACK ][1] = p[4];
        vertices[ CubeSphereFaceType::BACK ][2] = p[7];
        vertices[ CubeSphereFaceType::BACK ][3] = p[6];

        vertices[ CubeSphereFaceType::LEFT ][0] = p[1];
        vertices[ CubeSphereFaceType::LEFT ][1] = p[5];
        vertices[ CubeSphereFaceType::LEFT ][2] = p[6];
        vertices[ CubeSphereFaceType::LEFT ][3] = p[2];

        vertices[ CubeSphereFaceType::RIGHT ][0] = p[4];
        vertices[ CubeSphereFaceType::RIGHT ][1] = p[0];
        vertices[ CubeSphereFaceType::RIGHT ][2] = p[3];
        vertices[ CubeSphereFaceType::RIGHT ][3] = p[7];

        vertices[ CubeSphereFaceType::ABOVE ][0] = p[2];
        vertices[ CubeSphereFaceType::ABOVE ][1] = p[6];
        vertices[ CubeSphereFaceType::ABOVE ][2] = p[7];
        vertices[ CubeSphereFaceType::ABOVE ][3] = p[3];

        vertices[ CubeSphereFaceType::BELOW ][0] = p[5];
        vertices[ CubeSphereFaceType::BELOW ][1] = p[1];
        vertices[ CubeSphereFaceType::BELOW ][2] = p[0];
        vertices[ CubeSphereFaceType::BELOW ][3] = p[4];

        build( CubeSphereFaceType::FRONT, vertices[ CubeSphereFaceType::FRONT ] );
        build( CubeSphereFaceType::BACK , vertices[ CubeSphereFaceType::BACK  ] );
        build( CubeSphereFaceType::LEFT , vertices[ CubeSphereFaceType::LEFT  ] );
        build( CubeSphereFaceType::RIGHT, vertices[ CubeSphereFaceType::RIGHT ] );
        build( CubeSphereFaceType::ABOVE, vertices[ CubeSphereFaceType::ABOVE ] );
        build( CubeSphereFaceType::BELOW, vertices[ CubeSphereFaceType::BELOW ] );


        for ( int i = 0; i<CubeSphereFaceType::MAX_FACE_COUNT; i++ )
        {
            CubeSphereFaceType face = (CubeSphereFaceType)i;
            _face_rotations[ face ][ 0 ] = _faces[ face ]._xaxis;
            _face_rotations[ face ][ 1 ] = _faces[ face ]._yaxis;
            _face_rotations[ face ][ 2 ] = kege::normalize( kege::cross( _face_rotations[ face ][ 0 ].xyz(), _face_rotations[ face ][ 1 ].xyz() ) );
            _face_rotations[ face ][ 0 ].w = 0.0f;
            _face_rotations[ face ][ 1 ].w = 0.0f;
            _face_rotations[ face ][ 2 ].w = 0.0f;
        }

        p[0].z = 0.f;
        p[1].z = 0.f;
        p[2].z = 0.f;
        p[3].z = 0.f;

        kege::vec3 v[4];
        const int32_t mesh_width = 8;
        const int32_t w1 = mesh_width + 1;
        std::vector< kege::vec3 > vdata;
        vdata.resize( kege::sq( w1 ) );

        uint32_t count = 0;
        float step_length = 1.0f / float( mesh_width );
        for(float y = 0.0f; y <= 1.0f; y += step_length)
        {
            for(float x = 0.0f; x <= 1.0f; x += step_length)
            {
                v[0] = kege::lerp( p[1], p[0], x );
                v[1] = kege::lerp( p[2], p[3], x );
                v[2] = kege::lerp( v[0], v[1], y );
                vdata[ count++ ] = v[2];
                //std::cout << v[2] <<"\n";
            }
        }

        count = 0;
        struct QuadIndices{ uint32_t i[24]; };
        std::vector< QuadIndices > idata;
        idata.resize( kege::max( kege::sq( mesh_width / 2 ), 1) );
        for(int y = 0.0f; y < mesh_width; y+=2)
        {
            for(int x = 0.0f; x < mesh_width; x+=2)
            {
                idata[ count ].i[0] = x + y * w1;
                idata[ count ].i[1] = idata[ count ].i[0] + w1;
                idata[ count ].i[2] = idata[ count ].i[1] + 1;
                idata[ count ].i[3] = idata[ count ].i[2];
                idata[ count ].i[4] = idata[ count ].i[0] + 1;
                idata[ count ].i[5] = idata[ count ].i[0];

                //std::cout <<"{"<< idata[ count ].i[0] <<","<< idata[ count ].i[1] <<","<< idata[ count ].i[2]<<"}\n";
                //std::cout <<"{"<< idata[ count ].i[3] <<","<< idata[ count ].i[4] <<","<< idata[ count ].i[5]<<"}\n\n";

                idata[ count ].i[ 6] = idata[ count ].i[4] + 1;
                idata[ count ].i[ 7] = idata[ count ].i[4];
                idata[ count ].i[ 8] = idata[ count ].i[3];
                idata[ count ].i[ 9] = idata[ count ].i[2];
                idata[ count ].i[10] = idata[ count ].i[2] + 1;
                idata[ count ].i[11] = idata[ count ].i[6];

                //std::cout <<"{"<< idata[ count ].i[6] <<","<< idata[ count ].i[7] <<","<< idata[ count ].i[8]<<"}\n";
                //std::cout <<"{"<< idata[ count ].i[9] <<","<< idata[ count ].i[10] <<","<< idata[ count ].i[11]<<"}\n\n";

                idata[ count ].i[12] = idata[ count ].i[ 2];
                idata[ count ].i[13] = idata[ count ].i[ 1];
                idata[ count ].i[14] = idata[ count ].i[ 1] + w1;
                idata[ count ].i[15] = idata[ count ].i[14];
                idata[ count ].i[16] = idata[ count ].i[14] + 1;
                idata[ count ].i[17] = idata[ count ].i[12];

                //std::cout <<"{"<< idata[ count ].i[12] <<","<< idata[ count ].i[13] <<","<< idata[ count ].i[14]<<"}\n";
                //std::cout <<"{"<< idata[ count ].i[15] <<","<< idata[ count ].i[16] <<","<< idata[ count ].i[17]<<"}\n\n";

                idata[ count ].i[18] = idata[ count ].i[17];
                idata[ count ].i[19] = idata[ count ].i[16];
                idata[ count ].i[20] = idata[ count ].i[16] + 1;
                idata[ count ].i[21] = idata[ count ].i[20];
                idata[ count ].i[22] = idata[ count ].i[10];
                idata[ count ].i[23] = idata[ count ].i[18];

                //std::cout <<"{"<< idata[ count ].i[18] <<","<< idata[ count ].i[19] <<","<< idata[ count ].i[20]<<"}\n";
                //std::cout <<"{"<< idata[ count ].i[21] <<","<< idata[ count ].i[22] <<","<< idata[ count ].i[23]<<"}\n\n";

                count++;
            }
        }

        uint32_t draw_count = mesh_width * mesh_width * 6;

        kege::Ref< core::Buffer > vbo = KEGE::core()->graphics()->createBuffer
        (
            core::Buffer::VERTEX,
            vdata.size() * sizeof( kege::vec3 ),
            vdata.data(),
            core::Buffer::STATIC
        );

        kege::Ref< core::Buffer > ibo = KEGE::core()->graphics()->createBuffer
        (
            core::Buffer::INDEX,
            idata.size() * sizeof( QuadIndices ),
            idata.data(),
            core::Buffer::STATIC
        );

        core::VertexArrayIndexed* vertexarray = new core::VertexArrayIndexed
        (
            {{ 0, Vertex::POSITION, Vertex::F16_3, 0 }},
            {{ vbo, int32_t(sizeof(kege::vec3)), core::VertexInputRate::PER_VERTEX }},
            new core::IndexBuffer( core::Index::UI32, 0, draw_count, ibo )
        );

        _geometry = new kege::Geometry( 0, draw_count, vertexarray );
    }

    const kege::Geometry* CubeSphereQuadtree::getGeometry()const
    {
        return _geometry.ref();
    }

    CubeSphereQuadtree::CubeSphereQuadtree
    (
        double radius, double resolution, uint32_t depth, uint32_t binding_index
    )
    :   _batch_mngr( binding_index, 4, 8, (uint32_t) sizeof( kege::mat24 ), "InstanceBatchBuffer" ) // max-size = pow( 4, 8 ) = 65536
    ,   _rotation( 1.0 )
    ,   _position( 0.0 )
    ,   _max_resolution( resolution )
    ,   _max_depth( depth )
    ,   _radius( radius )
    {}

    CubeSphereQuadtree::~CubeSphereQuadtree()
    {
        //_batch_mngr.clear();
    }

    CubeSphereQuadtree::CubeSphereQuadtree( uint32_t binding_index )
    :   _batch_mngr( binding_index, 4, 8, (uint32_t) sizeof( kege::mat24 ), "InstanceBatchBuffer" ) // max-size = pow( 4, 8 ) = 65536
    ,   _rotation( 1.0 )
    ,   _position( 0.0 )
    ,   _max_resolution( 1.f )
    ,   _max_depth( 5.f )
    ,   _radius( 1.f )
    {}



//    class CubeSphereLandscape : public Landscape
//    {
//    public:
//    };
}
