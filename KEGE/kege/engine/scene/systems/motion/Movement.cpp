//
//  Movement.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/21/23.
//

#include "../../systems/include/systems.hpp"
namespace kege{ namespace system{

    void Movement::update( const double& dms )
    {
        if ( _entities == nullptr ) return;
        for ( Entity entity : *_entities )
        {
            component::Movement* movement = entity.get< component::Movement >();
            component::Transform* transform = entity.get< component::Transform >();

            movement->velocity *= pow( 0.85, movement->damping );
            movement->velocity += movement->acceleration * dms;
            transform->position() += movement->velocity * dms;
        }
    }
    Movement::Movement( int priority )
    :   kege::EntitySystem( priority, "Movement" )
    {
        _components = createComponentBitmask< component::Transform, component::Movement >();
    }

}}
