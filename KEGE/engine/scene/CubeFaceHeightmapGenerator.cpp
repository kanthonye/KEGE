//
//  CubeFaceHeightmapGenerator.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/17/23.
//

#include "CubeSphereQuadtreeFace.hpp"
#include "CubeFaceHeightmapGenerator.hpp"
namespace kege
{
    void CubeFaceHeightmapGenerator::generatorHeightmap
    (
        const CubeFaceHeightmapGenerator::Params& params,
        std::vector< kege::Ref< core::Texture > > &textures
    )
    {
        const uint32_t params_bytesize = (uint32_t)sizeof( CubeFaceHeightmapGenerator::Params );

        kege::UniformLayout1 heightmaps[] =
        {
            {new kege::ImageStorageBinding( "Heightmap", 0, textures[ 0 ] )},
            {new kege::ImageStorageBinding( "Heightmap", 0, textures[ 1 ] )},
            {new kege::ImageStorageBinding( "Heightmap", 0, textures[ 2 ] )},
            {new kege::ImageStorageBinding( "Heightmap", 0, textures[ 3 ] )},
            {new kege::ImageStorageBinding( "Heightmap", 0, textures[ 4 ] )},
            {new kege::ImageStorageBinding( "Heightmap", 0, textures[ 5 ] )},
        };

        CubeFaceHeightmapGenerator::Params parameters[6] = {
            params, params, params, params, params, params
        };
        parameters[0].FACE_INDEX = 0;
        parameters[1].FACE_INDEX = 1;
        parameters[2].FACE_INDEX = 2;
        parameters[3].FACE_INDEX = 3;
        parameters[4].FACE_INDEX = 4;
        parameters[5].FACE_INDEX = 5;

        _primary_command_buffer->begin();
        _primary_command_buffer->pipelineBarrier( textures[ 0 ].ref() );
        _primary_command_buffer->pipelineBarrier( textures[ 1 ].ref() );
        _primary_command_buffer->pipelineBarrier( textures[ 2 ].ref() );
        _primary_command_buffer->pipelineBarrier( textures[ 3 ].ref() );
        _primary_command_buffer->pipelineBarrier( textures[ 4 ].ref() );
        _primary_command_buffer->pipelineBarrier( textures[ 5 ].ref() );
        std::vector<core::CommandBuffer *> secondary_command_buffers;
        const int workgroup_size_x = 25;
        const int workgroup_size_y = 25;
        const int total_thread_count_x = textures[ 0 ]->width();
        const int total_thread_count_y = textures[ 0 ]->height();
        const int group_count_x = ceil(total_thread_count_x / workgroup_size_x);
        const int group_count_y = ceil(total_thread_count_y / workgroup_size_y);
        for (int i=0; i < CubeSphereFaceType::MAX_FACE_COUNT; i++ )
        {
            core::CommandBuffer* command = _command_buffers[ i ].ref();
            command->begin();

            _heightmap_compute_shader->bind( command );
            _heightmap_compute_shader->bindUniformLayout( 0, &heightmaps[ i ] );
            _heightmap_compute_shader->bindUniformLayout( 1, &_cube_face_points );
            _heightmap_compute_shader->pushConstants( core::Shader::COMPUTE, 0, params_bytesize, &parameters[ i ] );
            _heightmap_compute_shader->despatch( total_thread_count_x, total_thread_count_y, 1 );

            //_normalmap_compute_shader->bind( command );
            //_normalmap_compute_shader->bindUniformLayout( 0, &heightmap_textures[ i ] );
            //_normalmap_compute_shader->pushConstants( core::Shader::COMPUTE, 0, sizeof(float), &params._normalmap_strength );
            //_normalmap_compute_shader->despatch( 16, 16, 1 );

            command->end();
            secondary_command_buffers.push_back( command );
        }
        _primary_command_buffer->executeCommands( secondary_command_buffers );
        _primary_command_buffer->imageTransitionComputeToGraphics( textures[ 0 ].ref() );
        _primary_command_buffer->imageTransitionComputeToGraphics( textures[ 1 ].ref() );
        _primary_command_buffer->imageTransitionComputeToGraphics( textures[ 2 ].ref() );
        _primary_command_buffer->imageTransitionComputeToGraphics( textures[ 3 ].ref() );
        _primary_command_buffer->imageTransitionComputeToGraphics( textures[ 4 ].ref() );
        _primary_command_buffer->imageTransitionComputeToGraphics( textures[ 5 ].ref() );
        _primary_command_buffer->end();
        KEGE::core()->graphics()->renderer()->submit( _primary_command_buffer.ref() );
    }

    std::vector< kege::Ref< core::Texture > > CubeFaceHeightmapGenerator::createCubeFaceTextures
    (
        uint32_t width, uint32_t height
    )
    {
        core::ImgFormat format = core::R32_FLOAT;
        core::Device* device = KEGE::core()->graphics()->device();

        kege::Ref< core::Sampler > sampler = device->createSampler
        (
            core::Sampler::LINEAR,
            core::Sampler::LINEAR,
            core::Sampler::CLAMP_TO_EDGE,
            core::Sampler::CLAMP_TO_EDGE,
            core::Sampler::CLAMP_TO_EDGE
        );

        std::vector< kege::Ref< core::Texture > > textures =
        {
            device->createTexture2d( width, height, 1, format, nullptr, true ),
            device->createTexture2d( width, height, 1, format, nullptr, true ),
            device->createTexture2d( width, height, 1, format, nullptr, true ),
            device->createTexture2d( width, height, 1, format, nullptr, true ),
            device->createTexture2d( width, height, 1, format, nullptr, true ),
            device->createTexture2d( width, height, 1, format, nullptr, true )
        };

        return textures;
    }

    CubeFaceHeightmapGenerator::CubeFaceHeightmapGenerator()
    {
        kege::ShaderLibrary* library = &KEGE::core()->graphics()->shaderLibrary();
        core::Device* device = KEGE::core()->graphics()->device();

        _heightmap_compute_shader = library->get( "heightmap-generator.shader" );
        if ( _heightmap_compute_shader == nullptr ) {
            throw std::runtime_error("error with createing heightmap-generator.shader");
        }
//        _normalmap_compute_shader = library->get( "normalmap-generator.shader" );
//        if ( _normalmap_compute_shader == nullptr ) {
//            throw std::runtime_error("error with createing normalmap-generator.shader");
//        }

        /**
         * my primary command buffer which will execute the commands of the secondary command buffers
         */
        _primary_command_buffer = device->createCommandBuffer( core::CmdQueueType::COMPUTE_QUEUE );

        /**
         * using secondary command buffer for parallel execution, each face executing independently
         */
        _command_buffers[ 0 ] = _primary_command_buffer->createSecondaryCommandBuffer();
        _command_buffers[ 1 ] = _primary_command_buffer->createSecondaryCommandBuffer();
        _command_buffers[ 2 ] = _primary_command_buffer->createSecondaryCommandBuffer();
        _command_buffers[ 3 ] = _primary_command_buffer->createSecondaryCommandBuffer();
        _command_buffers[ 4 ] = _primary_command_buffer->createSecondaryCommandBuffer();
        _command_buffers[ 5 ] = _primary_command_buffer->createSecondaryCommandBuffer();

        kege::vec4 p[ 8 ];
        float length = 1.0f;
        /**
         * front and back cube face quad vertices, which is necessary for building the entire cube
         */
        p[0] = kege::vec4(  length,  length,  length, 0.f );
        p[1] = kege::vec4( -length,  length,  length, 0.f );
        p[2] = kege::vec4( -length, -length,  length, 0.f );
        p[3] = kege::vec4(  length, -length,  length, 0.f );

        p[4] = kege::vec4(  length,  length, -length, 0.f );
        p[5] = kege::vec4( -length,  length, -length, 0.f );
        p[6] = kege::vec4( -length, -length, -length, 0.f );
        p[7] = kege::vec4(  length, -length, -length, 0.f );

        kege::mat44 vertices[ CubeSphereFaceType::MAX_FACE_COUNT ];

        /**
         * build cube face quad vertices
         */
        vertices[ CubeSphereFaceType::FRONT ][0] = p[0];
        vertices[ CubeSphereFaceType::FRONT ][1] = p[1];
        vertices[ CubeSphereFaceType::FRONT ][2] = p[2];
        vertices[ CubeSphereFaceType::FRONT ][3] = p[3];

        vertices[ CubeSphereFaceType::BACK ][0] = p[5];
        vertices[ CubeSphereFaceType::BACK ][1] = p[4];
        vertices[ CubeSphereFaceType::BACK ][2] = p[7];
        vertices[ CubeSphereFaceType::BACK ][3] = p[6];

        vertices[ CubeSphereFaceType::LEFT ][0] = p[1];
        vertices[ CubeSphereFaceType::LEFT ][1] = p[5];
        vertices[ CubeSphereFaceType::LEFT ][2] = p[6];
        vertices[ CubeSphereFaceType::LEFT ][3] = p[2];

        vertices[ CubeSphereFaceType::RIGHT ][0] = p[4];
        vertices[ CubeSphereFaceType::RIGHT ][1] = p[0];
        vertices[ CubeSphereFaceType::RIGHT ][2] = p[3];
        vertices[ CubeSphereFaceType::RIGHT ][3] = p[7];

        vertices[ CubeSphereFaceType::ABOVE ][0] = p[0];
        vertices[ CubeSphereFaceType::ABOVE ][1] = p[4];
        vertices[ CubeSphereFaceType::ABOVE ][2] = p[5];
        vertices[ CubeSphereFaceType::ABOVE ][3] = p[1];

        vertices[ CubeSphereFaceType::BELOW ][0] = p[7];
        vertices[ CubeSphereFaceType::BELOW ][1] = p[3];
        vertices[ CubeSphereFaceType::BELOW ][2] = p[2];
        vertices[ CubeSphereFaceType::BELOW ][3] = p[6];

        /**
         * Load cube face quad vertices into a GPU buffer
         */
        kege::Ref< core::Buffer > buffer = device->createBuffer
        (
            core::Buffer::STORAGE,
            6 * sizeof( kege::mat44 ), vertices,
            core::Buffer::DYNAMIC
        );

        _cube_face_points.setBinding( 0, new kege::BufferBinding( "CubeFacePoints", 0, buffer ) );
    }

    void CubeFaceHeightmapGenerator::waitForExecutionToFinish()
    {
        KEGE::core()->graphics()->renderer()->wait( _primary_command_buffer.ref() );
    }
}
