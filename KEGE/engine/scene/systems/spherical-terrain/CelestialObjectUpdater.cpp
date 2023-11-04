//
//  CelestialObjectUpdater.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/15/23.
//

#include "../../systems/include/systems.hpp"

namespace kege{namespace system{

    void CelestialObjectUpdater::render( const double& dms )
    {
        if ( _entities == nullptr ) return;

        kege::Entity entity;
        for ( kege::Entity entity : *_entities )
        {
            component::SphericalQuadtreeTerrain* terrain = entity.get< component::SphericalQuadtreeTerrain >();
            terrain->ref()->render( dms );
        }
    }

    void CelestialObjectUpdater::update( const double& dms )
    {
        if ( _entities == nullptr ) return;

        kege::Entity entity_camera = system::Camera::_entity;
        component::Transform* camera_transform = entity_camera.get< component::Transform >();

        kege::vec3 camera_position = camera_transform->_position;

        for ( const auto& entity : *_entities )
        {
            kege::Entity terrain_entity = entity;
            component::SphericalQuadtreeTerrain* terrain = terrain_entity.get< component::SphericalQuadtreeTerrain >();
            component::Transform* transform = terrain_entity.get< component::Transform >();

            terrain->ref()->position() = transform->_position;
            terrain->ref()->rotation() = transform->_rotation;
            terrain->ref()->update( dms, camera_position );
        }
    }

    CelestialObjectUpdater::CelestialObjectUpdater( int priority )
    :   kege::EntitySystem(0, "CelestialObjectUpdater")
    {
        _components = createComponentBitmask< component::SphericalQuadtreeTerrain >();
    }

}}
