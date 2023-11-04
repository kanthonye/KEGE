//
//  AtmosphereRenderTechnique.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/10/23.
//

#include "../../../include/systems.hpp"
#include "RenderTarget.hpp"
#include "AtmosphereRenderTechnique.hpp"
namespace kege
{
    void AtmosphereRenderTechnique::execute( core::CommandBuffer* command )
    {
        if ( !_entities ) return;

        _shader->bind( command );
        _shader->bindUniformLayout( 0, system::Camera::_resource.ref() );
        _shader->bindUniformLayout( 2, _render_phase->getInputRenderTarget()->getCurrentUniformLayout() );

        ObjectMatrices object_matrices;
        uint32_t sizeof_object_matrices = sizeof( ObjectMatrices );
        uint32_t sizeof_atmosphere = sizeof( component::Atmosphere );

        for ( const auto& id : *_entities )
        {
            Entity entity = id;
            component::Atmosphere* atmosphere = entity.get< component::Atmosphere >();
            _atmosphere_data.getBinding(0)->getBuffer(0)->copyToSystemMemory( sizeof_atmosphere, atmosphere );

            if ( entity.has< component::Child >() )
            {
                component::Child* child = entity.get< component::Child >();
                object_matrices._transform = buildModelMatrix( child->_transform );
                object_matrices._rotation = child->_transform._rotation;
            }
            else
            {
                component::Transform* transform = entity.get< component::Transform >();
                object_matrices._transform = buildModelMatrix( *transform );
                object_matrices._rotation = transform->_rotation;
            }

            _shader->pushConstants( core::Shader::VERTEX, 0, sizeof_object_matrices, &object_matrices );
            _shader->bindUniformLayout( 1, &_atmosphere_data );
            command->draw( 4, 1, 0, 0 );
        }
    }

    AtmosphereRenderTechnique::~AtmosphereRenderTechnique()
    {
        _entities = nullptr;
    }

    AtmosphereRenderTechnique::AtmosphereRenderTechnique()
    :   kege::RenderTechnique( "post-process-atmosphere.shader" )
    ,   _entities( nullptr )
    {
        _entities = Entity::egm()->get( createComponentBitmask< component::Transform, component::Atmosphere >()._bitmask );

        _atmosphere_data.setBinding
        (
            0, // store binding at index 0
            new kege::BufferBinding
            (
                "AtmosphereData",  // binding target name. note this must match the name of the target in the shader
                0, // the binding index in shader. note this must match the index of the target in the shader
                KEGE::core()->graphics()->createBuffer // the buffer to bind at the target index
                (
                    core::Buffer::UNIFORM,
                    sizeof( component::Atmosphere ),
                    nullptr, core::Buffer::DYNAMIC
                )
            )
        );
    }

}
