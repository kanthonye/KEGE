//
//  FloatingOrigin.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/26/23.
//


#include "ECS.hpp"
#include "../../systems/include/systems.hpp"
#include "../../systems/components/components.hpp"

namespace kege{namespace system{

    void FloatingOrigin::update( const double& dms )
    {
        if ( _entities == nullptr ) return;

        component::Transform* cam = system::Camera::_entity.get< component::Transform >();
        for ( kege::Entity entity : *_entities )
        {
            if ( entity != system::Camera::_entity )
            {
                component::Transform* t = entity.get< component::Transform >();
                t->_position -= cam->_position;
            }
        }
        cam->_position = 0.0;
    }

    FloatingOrigin::FloatingOrigin( int priority )
    :   kege::EntitySystem( priority, "FloatingOrigin" )
    {
        _components = createComponentBitmask< component::Transform, component::RootObject >();
    }

    FloatingOrigin::~FloatingOrigin()
    {
    }
}}
