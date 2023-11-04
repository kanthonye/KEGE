//
//  Shader.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/17/23.
//

#include "../vulkan/Shader.hpp"
#include "../vulkan/Device.hpp"
#include "../vulkan/CommandBuffer.hpp"

namespace kege{namespace vk{

    void Shader::render( kege::Geometry* geometry, uint32_t first_instance, uint32_t instance_count )const
    {
        geometry->draw( _command, first_instance, instance_count );
    }

    void Shader::bind( kege::Geometry* geometry )const
    {
        geometry->bind( _command );
    }

    void Shader::bindUniformLayoutByName( const char* name, const core::UniformLayout* layout )
    {
        int32_t index = getShaderResourceIndex( name );
        if ( index < 0 )
        {
            KEGE_ERROR("caller -> Shader::bindUniformLayout(): @param -> %s is an invalid resource name.", name );
            return;
        }
        bindUniformLayout( index, layout );
    }

    void Shader::bindUniformLayout( uint32_t setindex, const core::UniformLayout* layout )
    {
        if ( !layout->getUniformHandle() )
        {
            vk::PipelineLayoutManager* mgr = vk::Device::get()->getPipelineLayoutManager();
            layout->setUniformHandle( mgr->createUniformHandle( layout ) );
        }
        _command->bindUniformLayout( setindex, layout, this );
    }

    bool checkShaderStageFlag( uint32_t stages, uint32_t flag )
    {
        return (stages & flag) == flag;
    }
    
    VkShaderStageFlags getShaderStageFlags( uint32_t stages )
    {
        VkShaderStageFlags shader_stage_flags = 0;
        if ( checkShaderStageFlag(stages, core::Shader::VERTEX         ) ) shader_stage_flags |= VK_SHADER_STAGE_VERTEX_BIT;
        if ( checkShaderStageFlag(stages, core::Shader::FRAGMENT       ) ) shader_stage_flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
        if ( checkShaderStageFlag(stages, core::Shader::TESS_CONTROL   ) ) shader_stage_flags |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        if ( checkShaderStageFlag(stages, core::Shader::TESS_EVALUATION) ) shader_stage_flags |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        if ( checkShaderStageFlag(stages, core::Shader::GEOMETRY       ) ) shader_stage_flags |= VK_SHADER_STAGE_GEOMETRY_BIT;
        if ( checkShaderStageFlag(stages, core::Shader::COMPUTE        ) ) shader_stage_flags |= VK_SHADER_STAGE_COMPUTE_BIT;
        return shader_stage_flags;
    }

    void Shader::pushConstants( uint32_t stages, uint32_t offset, uint32_t size, const void* data )
    {
        VkShaderStageFlags shader_stage_flagss = getShaderStageFlags( stages );
        _command->pushConstants( _pipeline_layout->handle(), shader_stage_flagss, offset, size, data );
    }

    int32_t Shader::getShaderResourceIndex( const char* name )const
    {
        return -1;
    }

    void Shader::bind( core::CommandBuffer* command )const
    {
        _command = command->vulkan();
        _command->bindShader( this );
    }

    const vk::PipelineLayout* Shader::pipelineLayout()const
    {
        return _pipeline_layout;
    }

    const VkPipeline Shader::handle()const
    {
        return _handle;
    }
    
    const vk::Shader* Shader::vulkan()const
    {
        return this;
    }

    vk::Shader* Shader::vulkan()
    {
        return this;
    }

    void Shader::destruct()
    {
        if ( _handle )
        {
            vk::Device::get()->waitIdle();
            vkDestroyPipeline( vk::Device::get()->logicalDevice(), _handle, nullptr );
            _pipeline_layout = VK_NULL_HANDLE;
            _handle = VK_NULL_HANDLE;
        }
    }
    
    Shader:: ~ Shader()
    {
        destruct();
    }
    
    Shader::Shader()
    :   _pipeline_layout( VK_NULL_HANDLE )
    {}

}}
