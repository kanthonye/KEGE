//
//  CameraRotation.hpp
//  kege-rendering-engine
//
//  Created by Kenneth A Esdaile on 10/3/21.
//

#ifndef KEGE_SYSTEM_CAMERA_ROTATION
#define KEGE_SYSTEM_CAMERA_ROTATION

#include "Rotation.hpp"
#include "Transform.hpp"
#include "../../ecs/EntitySystem.hpp"

namespace kege{ namespace system{

    class CameraRotationX : public kege::EntitySystem
    {
    public:

        void update( const double& dms );
        CameraRotationX( int priority );
    };

}}


namespace kege{ namespace system{

    class CameraRotationY : public kege::EntitySystem
    {
    public:

        void update( const double& dms );
        CameraRotationY( int priority );
    };

}}


namespace kege{ namespace system{

    class CameraRotationZ : public kege::EntitySystem
    {
    public:

        void update( const double& dms );
        CameraRotationZ( int priority );
    };

}}

#endif /* KEGE_SYSTEM_CAMERA_ROTATION */
