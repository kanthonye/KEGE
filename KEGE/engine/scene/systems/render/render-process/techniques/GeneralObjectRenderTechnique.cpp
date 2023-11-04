//
//  GeneralObjectRenderTechnique.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/24/23.
//

#include "../../../include/systems.hpp"
#include "../techniques/GeneralObjectRenderTechnique.hpp"
namespace kege
{
    void GeneralObjectRenderTechnique::execute( core::CommandBuffer* command )
    {
        if ( !_entities ) return;
        _shader->bind( command );

        Entity entity;
        component::Child* child;
        kege::ObjectMatrices param;
        component::Transform* transform;
        component::Renderable* renderable;
        kege::Material* material;
        kege::Geometry* geometry;

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

            renderable = entity.get< component::Renderable >();
            material = renderable->_mesh->material();
            geometry = renderable->_mesh->geometry();

            //_shader->bindUniformLayout( 1, effect.ref() );
            _shader->pushConstants( core::Shader::VERTEX, 0, sizeof( ObjectMatrices ), &param );
            geometry->bind( command );
            geometry->draw( command, 0, 1 );
        }
    }

    GeneralObjectRenderTechnique::~GeneralObjectRenderTechnique()
    {
        _entities = nullptr;
    }

    GeneralObjectRenderTechnique::GeneralObjectRenderTechnique()
    :   kege::RenderTechnique( "general-object.shader" )
    ,   _entities( nullptr )
    {
        kege::Bitmask signature = createComponentBitmask< component::Transform, component::Renderable >();
        _entities = Entity::egm()->get( signature._bitmask );
    }
}
