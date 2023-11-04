//
//  MovementControl.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/21/23.
//

#ifndef KEGE_MOVEMENT_CONTROL_SYSTEM_HPP
#define KEGE_MOVEMENT_CONTROL_SYSTEM_HPP

#include "../../ecs/EntitySystem.hpp"

namespace kege{ namespace system{

    class MovementControl : public kege::EntitySystem
    {
    public:

        void update( const double& dms );
        MovementControl( int priority );
    };

}}


namespace kege{ namespace component{

    struct MotionControl
    {
        MotionControl()
        :   _speed( 2.5f )
        {}

        kege::fvec3 _speed;
        kege::fvec3 _angles;
        kege::fvec3 _sensitivity;
    };

}}
#endif /* KEGE_MOVEMENT_CONTROL_SYSTEM_HPP */
