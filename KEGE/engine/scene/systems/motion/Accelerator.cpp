//
//  Accelerator.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/25/23.
//


#include "../../systems/include/systems.hpp"

namespace kege{ namespace system{

    void Accelerator::update( const double& dms )
    {
        if ( _entities == nullptr ) return;
        kege::vec3 acceleration;
        for ( Entity entity : *_entities )
        {
            component::LinearMotion* linear = entity.get< component::LinearMotion >();
            component::Mass* mass = entity.get< component::Mass >();
            linear->_acceleration = linear->_netforce * mass->_invmass;
            linear->_netforce = 0.0f;
        }
    }
    
    Accelerator::Accelerator( int priority )
    :   kege::EntitySystem( priority, "Accelerator" )
    {
        _components = createComponentBitmask< component::Mass, component::LinearMotion >();
    }

}}
