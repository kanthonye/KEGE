//
//  transform.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/2/23.
//

#ifndef KEGE_TRANSFORM_HPP
#define KEGE_TRANSFORM_HPP
#include "../math/vec3.hpp"
#include "../math/mat33.hpp"
#include "../math/mat44.hpp"
namespace kege
{
    struct transform
    {
        transform(const kege::fmat33& rotation, const kege::fvec3& position, const kege::fvec3& scale)
        :   _rotation( rotation )
        ,   _position( position )
        ,   _scale( scale )
        ,   _world_up( 0, 1, 0)
        {}

        transform()
        :   _rotation( 1.0 )
        ,   _position( 0.0 )
        ,   _scale( 1.0 )
        ,   _world_up( 0, 1, 0)
        {}

        const kege::mat33& rotation()const;
        const kege::vec3& position()const;
        const kege::vec3& scale()const;
        const kege::vec3& worldUp()const;
        kege::mat33& rotation();
        kege::vec3& position();
        kege::vec3& scale();

        kege::vec3& worldUp();
        kege::vec3& forward();
        kege::vec3& right();
        kege::vec3& up();
        
        kege::fmat33 _rotation;
        kege::fvec3 _position;
        kege::fvec3 _scale;
        kege::fvec3 _world_up;
    };

    inline kege::transform operator*( const kege::transform& a, const kege::transform& b )
    {
        return kege::transform
        (
            a._rotation * b._rotation,
            a._position + a._rotation * ( a._scale * b._position ),
            a._scale * b._scale
        );
    }

    inline kege::vec3 operator*( const kege::transform& a, const kege::vec3& v )
    {
        return a._position + a._rotation * ( a._scale * v );
    }


    inline kege::mat44 buildModelMatrix( const kege::transform& t )
    {
        return kege::mat44
        (
            kege::vec4( t._scale[0] * t._rotation[0],  0.0f ),
            kege::vec4( t._scale[1] * t._rotation[1],  0.0f ),
            kege::vec4( t._scale[2] * t._rotation[2],  0.0f ),
            kege::vec4( t._scale    * t._position,     1.0f )
        );
    }
}
#endif /* KEGE_TRANSFORM_HPP */
