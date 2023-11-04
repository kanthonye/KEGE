//
//  CameraControl.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/21/23.
//

#ifndef KEGE_SYSTEM_CAMERA_CONTROL
#define KEGE_SYSTEM_CAMERA_CONTROL

#include "../../ecs/EntitySystem.hpp"

namespace kege{ namespace system{

    class CameraControl : public kege::EntitySystem
    {
    public:

        void update( const double& dms );
        CameraControl( int priority );
    };

}}
#endif /* KEGE_SYSTEM_CAMERA_CONTROL */
