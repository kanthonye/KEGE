//
//  vec2.cpp
//  KEGE_VECTOR_MATH
//
//  Created by Kenneth A Esdaile on 4/9/20.
//  Copyright © 2020 Kenneth A Esdaile. All rights reserved.
//
namespace kege{
    
    template< typename T > const Vec2< T > Vec2< T >::xaxis(1,0);
    template< typename T > const Vec2< T > Vec2< T >::yaxis(0,1);
    template< typename T > const Vec2< T > Vec2< T >::zero(0,0);
    template< typename T > const Vec2< T > Vec2< T >::one(1,1);
    
    template< typename T > inline Vec2< T > abs2(const Vec2< T >& v)
    {
        return { kege::abs( v.a[ 0 ] ), kege::abs( v.a[ 1 ] ) };
    }

    template< typename T > inline Vec2< T > floor2(const Vec2< T >& v)
    {
        return {  ::floor( v.a[ 0 ] ), ::floor( v.a[ 1 ] ) };
    }

    template< typename T > inline Vec2< T > fract2(const Vec2< T >& v)
    {
        return {  fract( v.a[ 0 ] ), fract( v.a[ 1 ] ) };
    }

    /** inline function. compute the dot product of a vectors. */
    template< typename T > inline double dot(const Vec2< T > &a, const Vec2< T > &b)
    {
        return a & b;
    }
    
    /*! inline function. compute the cross prouduct of two vectors. */
    template< typename T > inline T cross2(const Vec2< T > &a, const Vec2< T > &b)
    {
        return ( a.y || b.z );
    }
    
    /** inline function. compute the distance between two Vec3tors. */
    template< typename T > inline double distsq (const Vec2< T > &a, const Vec2< T > &b)
    {
        return sq( a.a[ 0 ] - b.a[ 0 ] ) + sq( a.a[ 1 ] - b.a[ 1 ] );
    }

    /** inline function. compute the distance between two Vec3tors. */
    template< typename T > inline double distant(const Vec2< T > &a, const Vec2< T > &b)
    {
        return sqrt( distsq( a,b ) );
    }

    /** inline function. compute the math::square magnitude of a Vec3tors. */
    template< typename T > inline double magnsq (const Vec2< T >& v)
    {
        return v & v;
    }

    /** inline function. compute the magnitude of a Vec3tors. */
    template< typename T > inline T magn(const Vec2< T >& v)
    {
        return sqrt( v & v );
    }

    template< typename T > Vec2< T > normalize2(const Vec2< T > &a)
    {
        return a * kege::invrs( magn( a ) );
    }

    /** compute the scalar projection of one Vec3tors onto another */
    template< typename T > inline double scalar_proj(const Vec2< T > &a, const Vec2< T > &b)
    {
        return (a & b) * kege::invrs( magn(b) );
    }

    /** compute the Vec3tor projection of one Vec3tors onto another */
    template< typename T > inline Vec2< T > Vec3_proj2(const Vec2< T > &a, const Vec2< T > &b)
    {
        return a * (a & b) * kege::invrs( magn(b) );
    }

    /** linear interpolate between two Vec3tor resulting the mix between them */
    template<typename T> Vec2<T> mix2(const Vec2<T>& from, const Vec2<T>& to, double t)
    {
        return from * (1.0 - t) + to * t;
    }

    /** compute the center point given a certain number of points */
    template< typename T > Vec2< T > center(const Vec2< T >* a, const int size)
    {
        Vec2< T > sum(0.0f);
        for (int i=0; i<size; i++) sum += a[i];
        return sum * (1.0 / double(size));
    }
    
    /** compute the center point given a certain number of points */
    template< typename T > Vec2< T > center(const Vec2< T >& a, const Vec2< T >& b, const Vec2< T >& c)
    {
        return (a + b + c) / T(0x3);
    }
    
    template< typename T > std::ostream& operator <<( std::ostream& os, const Vec2< T >& v)
    {
        int w = 10;
        int p = 4;
        os << "{ " << std::setw(w) << std::setprecision( p ) << v[ 0 ] << ", " << std::setw(w) << std::setprecision( p ) << v[ 1 ] << " }\n";
        return os;
    }
    
//    template< typename T > inline Vec2< T > abs( const Vec2< T >& a )
//    {
//        return Vec2< T >( abs(a.x), abs(a.y) );
//    }
//    
//    template< typename T > inline Vec2< T > floor( const Vec2< T >& v )
//    {
//        return Vec2< T >(::floor(v.x), ::floor(v.y), ::floor(v.z));
//    }
//    
//    template< typename T > inline Vec2< T > fract( const Vec2< T >& v )
//    {
//        return Vec2< T >( fract(v.x), fract(v.y) );
//    }
//    
//    /** inline function. compute the dot product of a vectors. */
//    template< typename T > inline double dot( const Vec2< T > &a, const Vec2< T > &b )
//    {
//        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
//    }
//    
//    /** inline function. compute the math::square magnitude of a vectors. */
//    template< typename T > inline double magnsq ( const Vec2< T >& v )
//    {
//        return dot( v,v );
//    }
//    
//    /** inline function. compute the magnitude of a vectors. */
//    template< typename T > inline double magn( const Vec2< T >& v )
//    {
//        return sqrt( dot( v,v ) );
//    }
//    
//    template< typename T > inline Vec2< T > normalize( const Vec2< T > &a)
//    {
//        return a * invrs( magn( a ) );
//    }
//    
//    /*! inline function. compute the cross prouduct of two vectors. */
//    template< typename T > inline double cross( const Vec2< T > &a, const Vec2< T > &b )
//    {
//        return (a.x*b.y - a.y*b.x);
//    }
//    
//    /*! inline function. compute the cross prouduct of two vectors. */
//    template< typename T > inline double triple_product (const Vec2< T > &a, const Vec2< T > &b, const Vec2< T > &c)
//    {
//        return a.x*(b.y*c.z - b.z*c.y) - a.y*(b.z*c.x - b.x*c.z) + a.z*(b.x*c.y - b.y*c.x);
//    }
//    
//    /** inline function. compute the distance between two vectors. */
//    template< typename T > inline double distant ( const Vec2< T > &a, const Vec2< T > &b )
//    {
//        return sqrt( sq(a.x-b.x) + sq(a.y-b.y) );
//    }
//    
//    /** inline function. compute the projection of one vectors onto another, return it magnitude. */
//    template< typename T > inline double scalar_proj( const Vec2< T > &a, const Vec2< T > &b )
//    {
//        return dot(a,b) * invrs( magn(b) );
//    }
//    
//    template< typename T > inline Vec2< T > vec_proj( const Vec2< T > &a, const Vec2< T > &b )
//    {
//        return a * dot(a,b) * invrs( magn(b) );
//    }
//    
//    /*! compute the cross prouduct of two vectors, then normalize it. */
//    template< typename T > inline Vec2< T > orthonormal(Vec2< T > const&B, Vec2< T > const&C)
//    {
//        return normalize( cross( B, C ) );
//    }
//    
//    /*! compute the face normal of triangle a,b,c. */
//    template< typename T > inline Vec2< T > face_normal(const Vec2< T > &a, const Vec2< T > &b, const Vec2< T > &c)
//    {
//        return cross(b - a, c - a);
//    }
//    
//    template< typename T > inline Vec2< T > face_orthonormal(const Vec2< T > &a, const Vec2< T > &b, const Vec2< T > &c)
//    {
//        return orthonormal(b - a, c - a);
//    }
//    
//    /*! calculate the radian between two vector, and return the angle in radian. */
//    template< typename T > inline real radian(const Vec2< T > &a, const Vec2< T > &b)
//    {
//        return acos( dot( a,b ) * invrs( magn( a ) * magn( b ) ) );
//    }
//    
//    /*! calculate the angle between two vector, and return the angle in angle. */
//    template< typename T > inline real angle(const Vec2< T > &a, const Vec2< T > &b)
//    {
//        return rad2deg( radian( a,b ) );
//    }
//    
//    /*! compute the angle between a vectors and the zx-plane. */
//    template< typename T > inline float radian(const Vec2< T > &a)
//    {
//        return radian(a, Vec2< T >(0.0f, 1.0));
//    }
//    
//    /*! compute the angle between a vectors and the zx-plane. */
//    template< typename T > inline float angle(const Vec2< T > &a)
//    {
//        return rad2deg( radian(a) );
//    }
//    
//    /*! compute the false length of a vector : but very fast */
//    template< typename T > inline double psuedo_length(const Vec2< T > &a)
//    {
//        return abs(a.x) + abs(a.y);
//    }
//    
//    template< typename T > Vec2< T > center(const Vec2< T > *a, const int size)
//    {
//        Vec2< T > res(0.0f);
//        for (int i=0; i<size; i++) res += a[i];
//        return res * invrs(size);
//    }
//    
//    template<typename T> Vec2<T> mix(const Vec2<T>& from, const Vec2<T>& to, float t)
//    {
//        return from * (1.0f - t) + to * t;
//    }
}
