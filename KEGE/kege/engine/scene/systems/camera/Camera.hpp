//
//  Camera.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/21/23.
//

#ifndef KEGE_SYSTEM_CAMERA
#define KEGE_SYSTEM_CAMERA

#include "../../ecs/EntitySystem.hpp"

namespace kege{ namespace system{

    class Camera : public kege::EntitySystem
    {
    public:

        void render( const double& dms );
        void update( const double& dms );
        static kege::Entity& entity();
        Camera( int priority );
        ~Camera();

        static kege::Ref< core::UniformLayout > _resource;
        static kege::Entity _entity;
    };

}}

#endif /* KEGE_SYSTEM_CAMERA */
