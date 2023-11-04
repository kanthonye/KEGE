//
//  CelestialSurfaceRenderTechnique.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/14/23.
//

#include "Landscape.hpp"
#include "../../../include/systems.hpp"
#include "CelestialSurfaceRenderTechnique.hpp"

namespace kege
{
    void CelestialSurfaceRenderTechnique::drawGeometry( kege::Batch* batch, const kege::Geometry* geometry )
    {
        _shader->bindUniformLayout( INSTANCE_BATCH_BUFFER, batch );
        geometry->draw( _command_buffer, 0, batch->_drawcount );
    }

    void CelestialSurfaceRenderTechnique::setFaceRotationSSBO( const kege::mat34 orientations[6] )
    {
        _face_orientation_matrices.getBinding( 0 )->getBuffer( 0 )->copyToSystemMemory( 0, 6 * sizeof( kege::mat34 ), orientations );
        _shader->bindUniformLayout( FACE_ORIENTATION_MATRICES, &_face_orientation_matrices );
    }

    void CelestialSurfaceRenderTechnique::setInput(uint32_t input, const core::UniformLayout *asset)
    {
        _shader->bindUniformLayout( input, asset );
    }

    void CelestialSurfaceRenderTechnique::bindGeometry( const kege::Geometry* geometry )
    {
        geometry->bind( _command_buffer );
    }

    void CelestialSurfaceRenderTechnique::execute( core::CommandBuffer* command )
    {
        if ( !_entities ) return;
        _shader->bind( command );
        _command_buffer = command;
        _shader->bindUniformLayout( CAMERA_TRANSFORM, system::Camera::_resource.ref() );

        Entity entity;
        kege::ObjectMatrices param;
        component::Child* child;
        component::Transform* transform;
        component::Landscape* landscape;
        uint32_t size = sizeof( ObjectMatrices );

        for ( const auto& id : *_entities )
        {
            entity = id;
            if ( entity.has< component::Child >() )
            {
                child = entity.get< component::Child >();
                param._transform = buildModelMatrix( child->_transform );
                param._rotation = child->_transform._rotation;
            }
            else
            {
                transform = entity.get< component::Transform >();
                param._transform = buildModelMatrix( *transform );
                param._rotation = transform->_rotation;
            }

            _shader->pushConstants( core::Shader::VERTEX, 0, size, &param );

            landscape = entity.get< component::Landscape >();
            landscape->ref()->drawTerrain( this );
        }
    }

    CelestialSurfaceRenderTechnique::~CelestialSurfaceRenderTechnique()
    {
        _entities = nullptr;
    }

    CelestialSurfaceRenderTechnique::CelestialSurfaceRenderTechnique()
    :   kege::RenderTechnique( "cube-sphere-quadtree-landscape.shader" )
    ,   _entities( nullptr )
    {
        kege::Bitmask signature = createComponentBitmask< component::Transform, component::Landscape >();
        _entities = Entity::egm()->get( signature._bitmask );

        kege::Ref< core::Buffer > buffer = KEGE::core()->graphics()->createBuffer
        (
            core::Buffer::STORAGE,
            6 * sizeof( kege::mat34 ),
            nullptr,
            core::Buffer::DYNAMIC
        );

//        kege::UniformLayout1( "BatchBuffer", 0, nullptr );
//        _shader->getUniformLayoutIndex( "" );
        _face_orientation_matrices.setBinding( 0, new kege::BufferBinding( "FaceOrientationBuffer", 0, buffer ) );
        //_orientation_matrices.setCount( 6 );
    }

}
