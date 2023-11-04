//
//  Camera.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/21/23.
//

#ifndef Camera_hpp
#define Camera_hpp
#include "cgm.hpp"
#include "Ref.hpp"

namespace kege
{
    struct Projection : public kege::RefCounter
    {
        enum Type{ PERSPECTIVE, ORTHOGRAPHIC };

        Projection( Projection::Type type ): _type( type ) {};

        virtual kege::fmat44 get()const = 0;
        virtual float aspectRatio()const{return 0;}
        virtual float fov()const{return 0;}
        virtual float near()const{return 0;}
        virtual float far()const{return 0;}

        virtual ~Projection(){};

        Projection::Type _type;
    };
}


namespace kege
{
    struct PerspectiveProjection : public kege::Projection
    {
        PerspectiveProjection
        (
             float aspect_ratio,
             float fov,
             float near,
             float far
        )
        :   kege::Projection( kege::Projection::PERSPECTIVE )
        ,   _aspect_ratio( aspect_ratio )
        ,   _fov( fov )
        ,   _near( near )
        ,   _far( far )
        {}

        kege::fmat44 get()const{ return kege::perspproj( _fov, _aspect_ratio, _near, _far ); }
        float aspectRatio()const{ return _aspect_ratio; }
        float near()const{ return _aspect_ratio; }
        float far()const{ return _aspect_ratio; }
        float fov()const{ return _aspect_ratio; }


        float _aspect_ratio;
        float _fov;
        float _near;
        float _far;
    };
}

namespace kege
{
    struct OrthographicProjection : public kege::Projection
    {
        OrthographicProjection(): kege::Projection( kege::Projection::ORTHOGRAPHIC ) {};
        kege::fmat44 get()const{ return kege::orthoproj( _left, _right, _bottom, _top, _near, _far ); }
        float aspectRatio()const{ return 1; }
        float near()const{ return _near; }
        float far()const{ return _far; }
        float fov()const{ return 45; }
        float _left;
        float _right;
        float _top;
        float _bottom;
        float _near;
        float _far;
    };
}

#endif /* Camera_hpp */
