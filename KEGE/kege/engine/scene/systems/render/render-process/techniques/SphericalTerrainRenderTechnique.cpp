//
//  SphericalTerrainRenderTechnique.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/24/23.
//

#include "../../../include/systems.hpp"
#include "../techniques/SphericalTerrainRenderTechnique.hpp"
namespace kege
{
    void SphericalTerrainRenderTechnique::execute( core::CommandBuffer* command )
    {
        if ( !_entities ) return;
        _shader->bind( command );
        _shader->bindUniformLayout( CAMERA_TRANSFORM, system::Camera::_resource.ref() );
        //_render_phase->_input_target

        Entity entity;
        kege::ObjectMatrices param;
        component::Child* child;
        component::Transform* transform;
        component::SphericalQuadtreeTerrain* terrain;
        uint32_t size = sizeof( ObjectMatrices );

        const std::vector< Ref< kege::DrawInstanceBuffer > >* buffers;
        for ( const auto& id : *_entities )
        {
            entity = id;
            if ( entity.has< component::Child >() )
            {
                child = entity.get< component::Child >();
                param._transform = buildModelMatrix( child->_transform );
                param._rotation = child->_transform._rotation;
            }
            else if ( entity.has< component::Transform >() )
            {
                transform = entity.get< component::Transform >();
                param._transform = buildModelMatrix( *transform );
                param._rotation = transform->_rotation;
            }
            else
            {
                param._transform = kege::mat44( 1.0f );
                param._rotation = kege::mat33( 1.0f );
            }

            terrain = entity.get< component::SphericalQuadtreeTerrain >();
            _shader->pushConstants( core::Shader::VERTEX, 0, size, &param );
            for(int i=0; i<6; i++)
            {
                kege::SphericalTerrainGeometry* geometry = terrain->ref()->geometries( i );
            
                buffers = &geometry->getDrawInstanceBuffers();
                for (int i=0; i<buffers->size(); i++)
                {
                    _shader->bindUniformLayout( INSTANCE_BATCH_BUFFER, buffers->at( i ).ref() );
                    _shader->bind( geometry );
                    _shader->render( geometry, 0, 1 );
                }
            }
        }
    }

    SphericalTerrainRenderTechnique::~SphericalTerrainRenderTechnique()
    {
        _entities = nullptr;
    }

    SphericalTerrainRenderTechnique::SphericalTerrainRenderTechnique()
    :   kege::RenderTechnique( "spherical-terrain.shader" )
    ,   _entities( nullptr )
    {
        kege::Bitmask signature = createComponentBitmask< component::Transform, component::SphericalQuadtreeTerrain >();
        _entities = Entity::egm()->get( signature._bitmask );
    }
    
}
