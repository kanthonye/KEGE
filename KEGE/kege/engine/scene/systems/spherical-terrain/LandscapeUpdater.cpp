//
//  LandscapeUpdater.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/15/23.
//

#include "../../systems/include/systems.hpp"

namespace kege{namespace system{

    void LandscapeUpdater::render( const double& dms )
    {
        if ( _entities == nullptr ) return;

        kege::Entity entity;
        for ( const auto& id : *_entities )
        {
            entity = id;
            entity.get< component::Landscape >()->ref()->cullTerrain();
        }
    }
    void LandscapeUpdater::update( const double& dms )
    {
        if ( _entities == nullptr ) return;

        kege::Entity entity = system::Camera::_entity;
        if ( !entity ) return;

        kege::vec3 eye = entity.get< component::Transform >()->position();

        for ( const auto& id : *_entities )
        {
            entity = id;
            component::Landscape* landscape = entity.get< component::Landscape >();

            if ( entity.has< component::Child >() )
            {
                component::Child* child = entity.get< component::Child >();
                landscape->ref()->setPosition( child->_transform._position );
                landscape->ref()->setRotation( child->_transform._rotation );
            }
            else
            {
                component::Transform* transform = entity.get< component::Transform >();
                landscape->ref()->setPosition( transform->_position );
                landscape->ref()->setRotation( transform->_rotation );
            }

            landscape->ref()->updateTerrain( eye );
        }
    }

    LandscapeUpdater::LandscapeUpdater( int priority )
    :   kege::EntitySystem(0, "CelestialObjectUpdater")
    {
        _components = createComponentBitmask< component::Landscape, component::Transform >();
    }

}}
