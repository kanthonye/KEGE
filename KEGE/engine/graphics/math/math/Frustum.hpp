//
//  Frustum.hpp
//  kege-rendering-engine
//
//  Created by Kenneth A Esdaile on 11/4/21.
//

#ifndef Frustum_hpp
#define Frustum_hpp
#include "cgm.hpp"
namespace kege{
    
    template< typename T > class Frustum
    {
    public:
        
        enum Side{ LEFT, RIGHT, ABOVE, BELOW, FRONT, BACK, TOTAL };
        
        struct Plane
        {
            kege::Vec3< T > _center;
            kege::Vec3< T > _normal;
            double _distance;
            Side _side;
        };
        
    public:
            
        enum Point{ FTR, FBR, FTL, FBL, NTR, NBR, NTL, NBL };
        
        Frustum
        (
            const T& near,
            const T& far,
            const T& fov,
            const T& aspect_ratio,
            const kege::Vec3< T >& right,
            const kege::Vec3< T >& up,
            const kege::Vec3< T >& forward,
            const kege::Vec3< T >& position
        );
        
        
        void MakePlane
        (
            typename Frustum< T >::Plane& plane,
            const typename Frustum< T >::Side& side,
            const kege::Vec3< T >& a,
            const kege::Vec3< T >& b,
            const kege::Vec3< T >& c,
            const kege::Vec3< T >& d
        );
        
        double Distance( const kege::Vec3< T >& point, const Frustum< T >::Plane& plane )const;
        int TestRect( const kege::Vec3< T >& center, const T& w, const T& h, const T& d )const;
        int TestAABB( const kege::Vec3< T >& min, const kege::Vec3< T >& max )const;
        int TestSegment( const kege::Vec3< T >& a, const kege::Vec3< T >& b )const;
        int TestSphere( const kege::Vec3< T >& center, const T& radius )const;
        int TestPoint( const kege::Vec3< T >& point )const;
        
        const kege::Vec3< T >& GetForwardVector()const;
        Frustum();
        
    private:
        
        kege::Vec3< T > _vertices[ 8 ];
        kege::point3d _center;
        kege::point3d _eye;
        kege::vec3 _forward;
        double _radius;
        
        Plane _planes[ 6 ];
    };
}

namespace kege
{
    template< typename T > void Frustum< T >::MakePlane( typename Frustum< T >::Plane& plane, const typename Frustum< T >::Side& side, const kege::Vec3< T >& a, const kege::Vec3< T >& b, const kege::Vec3< T >& c, const kege::Vec3< T >& d )
    {
        plane._normal = normalize( cross( b - a, c - a ) );
        plane._center = (a + b + c + d) * 0.25;
        plane._distance = plane._normal.x*plane._center.x + plane._normal.y*plane._center.y + plane._normal.z*plane._center.z;
        plane._side = side;
    }
    
    template< typename T > Frustum< T >::Frustum( const T& near, const T& far, const T& fov, const T& aspect_ratio, const kege::Vec3< T >& right, const kege::Vec3< T >& up, const kege::Vec3< T >& forward, const kege::Vec3< T >& position )
    {
        T angle = tanf( deg2rad( fov * 0.5 ) );
        T near_height = near * angle;
        T far_height  = far  * angle;
        T near_width  = near_height * aspect_ratio;
        T far_width   = far_height  * aspect_ratio;
        
        dvec3 near_centr = position - forward * near;
        dvec3 far_centr = position - forward * far;
        
        far_height = abs(far_height);
        far_width = abs(far_width);
        
        _vertices[ FTR ] = far_centr + right * far_width + up * far_height;
        _vertices[ FBR ] = far_centr + right * far_width - up * far_height;
        _vertices[ FTL ] = far_centr - right * far_width + up * far_height;
        _vertices[ FBL ] = far_centr - right * far_width - up * far_height;
        
        _vertices[ NTR ] = near_centr + right * near_width + up * near_height;
        _vertices[ NBR ] = near_centr + right * near_width - up * near_height;
        _vertices[ NTL ] = near_centr - right * near_width + up * near_height;
        _vertices[ NBL ] = near_centr - right * near_width - up * near_height;
        
        _eye = position;
        _forward = forward;
        _center = (far_centr + near_centr) * 0.5;
        _radius = kege::distant( _center, _vertices[ FTR ] );
        
        MakePlane( _planes[ Frustum::LEFT  ], Frustum::LEFT , _vertices[ FTL ], _vertices[ NTL ], _vertices[ FBL ], _vertices[ NBL ] );
        MakePlane( _planes[ Frustum::RIGHT ], Frustum::RIGHT, _vertices[ FTR ], _vertices[ FBR ], _vertices[ NTR ], _vertices[ NBR ] );
        MakePlane( _planes[ Frustum::BELOW ], Frustum::BELOW, _vertices[ FBL ], _vertices[ NBL ], _vertices[ FBR ], _vertices[ NBR ] );
        MakePlane( _planes[ Frustum::ABOVE ], Frustum::ABOVE, _vertices[ FTL ], _vertices[ FTR ], _vertices[ NTL ], _vertices[ NTR ] );
        MakePlane( _planes[ Frustum::FRONT ], Frustum::FRONT, _vertices[ FBL ], _vertices[ FTL ], _vertices[ FTR ], _vertices[ FBR ] );
        MakePlane( _planes[ Frustum::BACK  ], Frustum::BACK , _vertices[ NBL ], _vertices[ NBR ], _vertices[ NTR ],_vertices[ NTL ] );
    }
    
    template< typename T > double Frustum< T >::Distance( const kege::Vec3< T >& point, const typename Frustum< T >::Plane& plane )const
    {
        return kege::dot( plane._normal, point - plane._center );
    }
    template< typename T > int Frustum< T >::TestSphere( const kege::Vec3< T >& center, const T& radius )const
    {
        for ( int i=0; i<4; i++ )
        {
            if ( this->Distance( center, _planes[i] ) < -radius )
            {
                return 0;
            }
        }
        return 1;
    }
    
    template< typename T > int Frustum< T >::TestRect( const kege::Vec3< T >& center, const T& w, const T& h, const T& d )const
    {
        if( TestPoint( kege::point3( center.x-w, center.y-h, center.z-d ) ) ) return true;
        if( TestPoint( kege::point3( center.x-w, center.y-h, center.z+d ) ) ) return true;
        if( TestPoint( kege::point3( center.x-w, center.y+h, center.z-d ) ) ) return true;
        if( TestPoint( kege::point3( center.x-w, center.y+h, center.z+d ) ) ) return true;
        if( TestPoint( kege::point3( center.x+w, center.y-h, center.z-d ) ) ) return true;
        if( TestPoint( kege::point3( center.x+w, center.y-h, center.z+d ) ) ) return true;
        if( TestPoint( kege::point3( center.x+w, center.y+h, center.z-d ) ) ) return true;
        if( TestPoint( kege::point3( center.x+w, center.y+h, center.z+d ) ) ) return true;
        return false;
    }
    
    template< typename T > int Frustum< T >::TestAABB( const kege::Vec3< T >& min, const kege::Vec3< T >& max )const
    {
        if( TestPoint( kege::point3( min.x, min.y, min.z ) ) ) return true;
        if( TestPoint( kege::point3( max.x, min.y, min.z ) ) ) return true;
        if( TestPoint( kege::point3( min.x, max.y, min.z ) ) ) return true;
        if( TestPoint( kege::point3( max.x, max.y, min.z ) ) ) return true;
        if( TestPoint( kege::point3( min.x, min.y, max.z ) ) ) return true;
        if( TestPoint( kege::point3( max.x, min.y, max.z ) ) ) return true;
        if( TestPoint( kege::point3( min.x, max.y, max.z ) ) ) return true;
        if( TestPoint( kege::point3( max.x, max.y, max.z ) ) ) return true;
        return false;
    }
    
    template< typename T > int Frustum< T >::TestPoint( const kege::Vec3< T >& point )const
    {
        if( kege::dot( _planes[ 0 ]._normal, point ) - _planes[ 0 ]._distance < 0 ) return 0;
        if( kege::dot( _planes[ 1 ]._normal, point ) - _planes[ 1 ]._distance < 0 ) return 0;
        if( kege::dot( _planes[ 2 ]._normal, point ) - _planes[ 2 ]._distance < 0 ) return 0;
        if( kege::dot( _planes[ 3 ]._normal, point ) - _planes[ 3 ]._distance < 0 ) return 0;
        if( kege::dot( _planes[ 4 ]._normal, point ) - _planes[ 4 ]._distance < 0 ) return 0;
        if( kege::dot( _planes[ 5 ]._normal, point ) - _planes[ 5 ]._distance < 0 ) return 0;
        return 1;
    }
    
    template< typename T > const kege::Vec3< T >& Frustum< T >::GetForwardVector()const
    {
        return _forward;
    }
    
    template< typename T > Frustum< T >::Frustum()
    {}
}
#endif /* Frustum_hpp */
