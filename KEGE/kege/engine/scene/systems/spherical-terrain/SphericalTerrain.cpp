//
//  SphericalQuadtreeTerrain.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/26/23.
//

#include "../../systems/include/systems.hpp"

namespace kege{namespace system{

    void SphericalQuadtreeTerrain::render( const double& dms )
    {
        if ( _entities == nullptr ) return;

        kege::Entity entity;
        //std::vector< kege::mat24 > instance( MAX_SPHERICAL_TERRAIN_INSTANCES );
        //uint32_t count = 0;
        for ( kege::Entity entity : *_entities )
        {
            component::SphericalQuadtreeTerrain* terrain = entity.get< component::SphericalQuadtreeTerrain >();
            terrain->ref()->render( dms );
//            for( uint32_t i=0; i<6; i++ )
//            {
//                SphericalTerrainGeometry* geometry = terrain->ref()->geometries( i );
//                collectVisibleSubTiles( geometry, instance.data(), count );
//            }
        }
    }

    void SphericalQuadtreeTerrain::update( const double& dms )
    {
        kege::Entity entity_camera = system::Camera::_entity;
        if ( _entities == nullptr || !entity_camera ) return;

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
//            for( uint32_t i=0; i<6; i++ )
//            {
//                SphericalTerrainGeometry* geometry = terrain->ref()->geometries( i );
//                updateSubTile( terrain->ref(), geometry->subtile(), camera_position );
//            }
        }
    }

    SphericalQuadtreeTerrain::SphericalQuadtreeTerrain( int priority )
    :   kege::EntitySystem(0, "SphericalQuadtreeTerrain")
    {
        _components = createComponentBitmask< component::SphericalQuadtreeTerrain >();
    }

}}
