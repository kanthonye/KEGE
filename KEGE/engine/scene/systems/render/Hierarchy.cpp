//
//  Hierarchy.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/3/23.
//

#include "../../systems/include/systems.hpp"

namespace kege{ namespace system{

    void Hierarchy::updateChildren( component::Child* parent )
    {
        if ( Entity::ecm()->has< component::Children >( parent->_entity ) )
        {
            component::Children* children = Entity::ecm()->get< component::Children >( parent->_entity );
            for (component::Child* child = Entity::head( children ) ; child != nullptr ; child = Entity::next( child ) )
            {
                if ( Entity::ecm()->has< component::Transform >( child->_entity ) )
                {
                    component::Transform* transform = Entity::ecm()->get< component::Transform >( child->_entity );
                    child->_transform = parent->_transform * (*transform);
                    updateChildren( child );
                }
            }
        }
    }

    void Hierarchy::update( const double& dms )
    {
        if ( _entities == nullptr ) return;

        for ( Entity entity : *_entities )
        {
            component::Transform* transform = entity.get< component::Transform >();
            component::Children* children = entity.get< component::Children >();
            for (component::Child* child = Entity::head( children ) ; child != nullptr ; child = Entity::next( child ) )
            {
                if ( Entity::ecm()->has< component::Transform >( child->_entity ) )
                {
                    component::Transform* t = Entity::ecm()->get< component::Transform >( child->_entity );
                    child->_transform = (*transform) * (*t);
                    updateChildren( child );
                }
            }
        }
    }

    Hierarchy::Hierarchy( int priority )
    :   kege::EntitySystem( priority, "Hierarchy" )
    {
        _components = createComponentBitmask< component::RootObject, component::Children, component::Transform >();
        //getEntitySet< component::SceneObject, component::Children, component::Transform >();
    }
}}
