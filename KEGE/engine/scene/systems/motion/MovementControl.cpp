//
//  MovementControl.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/21/23.
//

#include "../../systems/include/systems.hpp"

namespace kege{ namespace system{

    float chop( float a, float b, float c )
    {
        return (a < b )? c : a;
    }

    void MovementControl::update( const double& dms )
    {
        if ( _entities == nullptr ) return;
        for ( Entity entity : *_entities )
        {
            component::Input* input = entity.get< component::Input >();
            component::Transform* transform = entity.get< component::Transform >();
            component::LinearMotion* linear = entity.get< component::LinearMotion >();
            component::MotionControl* movement = entity.get< component::MotionControl >();

            kege::vec3 velocity = 0.0;
            velocity -= transform->forward() * input->ref()->get( kege::EVENT_MOVE_FORWARD   );
            velocity += transform->forward() * input->ref()->get( kege::EVENT_MOVE_BACKWARD  );
            velocity += transform->right() * input->ref()->get( kege::EVENT_MOVE_RIGHTWARD );
            velocity -= transform->right() * input->ref()->get( kege::EVENT_MOVE_LEFTWARD  );
            velocity += transform->up() * input->ref()->get( kege::EVENT_MOVE_UPWARD    );
            velocity -= transform->up() * input->ref()->get( kege::EVENT_MOVE_DOWNWARD  );

            linear->_velocity += velocity * movement->_speed;
        }
    }
    MovementControl::MovementControl( int priority )
    :   kege::EntitySystem( priority, "movement-control-system" )
    {
        _components = createComponentBitmask< component::Input, component::LinearMotion, component::MotionControl >();
    }
    
}}
