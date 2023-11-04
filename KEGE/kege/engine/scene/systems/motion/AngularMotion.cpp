//
//  AngularMotion.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/18/23.
//

#include "../../systems/include/systems.hpp"

namespace kege{ namespace system{

    void AngularMotion::update( const double& dms )
    {
        if ( _entities == nullptr ) return;
    }
    AngularMotion::AngularMotion( int priority )
    :   kege::EntitySystem( priority, "AngularMotion" )
    {
        _components = createComponentBitmask< component::Transform, component::AngularMotion >();
    }

}}
