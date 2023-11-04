//
//  LinearMotion.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/18/23.
//

#include "../../systems/include/systems.hpp"

namespace kege{ namespace system{

    void LinearMotion::update( const double& dms )
    {
        if ( _entities == nullptr ) return;
        kege::vec3 acceleration;
        for ( Entity entity : *_entities )
        {
            component::LinearMotion* linear = entity.get< component::LinearMotion >();
            component::Transform* transform = entity.get< component::Transform >();

            linear->_velocity *= pow( linear->_damping, 0.02 );
            clampToZero( linear->_velocity.x, EPSILON );
            clampToZero( linear->_velocity.y, EPSILON );
            clampToZero( linear->_velocity.z, EPSILON );

            linear->_velocity += linear->_acceleration * dms;
            transform->position() += linear->_velocity * dms;
        }
    }
    LinearMotion::LinearMotion( int priority )
    :   kege::EntitySystem( priority, "LinearMotion" )
    {
        _components = createComponentBitmask< component::Transform, component::LinearMotion >();
    }

}}
