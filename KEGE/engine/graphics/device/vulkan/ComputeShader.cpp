//
//  ComputeShader.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/3/23.
//
#include <shaderc/shaderc.h>

#include "../vulkan/utils.hpp"
#include "../vulkan/Device.hpp"
#include "../vulkan/ComputeShader.hpp"

namespace kege{namespace vk{

    VkResult ComputeShader::construct( const core::Shader::Info& info )
    {
        std::vector< char > source;
        if( !loadShaderSource( source, info._shader_stages[0].second.c_str() ) )
        {
            return VK_INCOMPLETE;
        }

        // Create ComputePipelineLayout
        {
            vk::PipelineLayoutManager* mgr = vk::Device::get()->getPipelineLayoutManager();

            SpvReflectShaderModule reflect_module;
            SpvReflectResult result = spvReflectCreateShaderModule( source.size(), source.data(), &reflect_module );
            if (result != SPV_REFLECT_RESULT_SUCCESS)
            {
                throw std::runtime_error( "Error creating shader module" );
            }

            std::vector< VkPushConstantRange > push_constants;
            parsePushConstantRange( VK_SHADER_STAGE_COMPUTE_BIT, reflect_module, push_constants );

            std::vector< kege::Ref< vk::DescriptorSetLayout > > descriptor_set_layouts = mgr->createDescriptorSetLayouts
            ( 
                VK_SHADER_STAGE_COMPUTE_BIT,
                reflect_module
            );

            // Destroy the reflection data when no longer required.
            spvReflectDestroyShaderModule( &reflect_module );

            // CREATE PIPELINE LAYOUT
            _pipeline_layout = mgr->createPipelineLayout( VK_PIPELINE_BIND_POINT_COMPUTE, descriptor_set_layouts, push_constants );
        }


            // create the vulakn shader module
        VkShaderModuleCreateInfo create_module_info{};
        create_module_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_module_info.pCode = reinterpret_cast<const uint32_t*>( source.data() );
        create_module_info.codeSize = source.size();
        VkShaderModule shader_module;
        VkResult result = vkCreateShaderModule( vk::Device::get()->logicalDevice(), &create_module_info, nullptr, &shader_module );
        if ( result != VK_SUCCESS )
        {
            KEGE_FATAL("%s: failed to create shader module!", vkGetError( result ) );
            return result;
        }

        VkPipelineShaderStageCreateInfo compute_shader_stage_info{};
        compute_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        compute_shader_stage_info.stage = VK_SHADER_STAGE_COMPUTE_BIT;
        compute_shader_stage_info.module = shader_module;
        compute_shader_stage_info.pName = "main";

        VkComputePipelineCreateInfo pipeline_info{};
        pipeline_info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        pipeline_info.layout = _pipeline_layout->handle();
        pipeline_info.stage = compute_shader_stage_info;

        if (vkCreateComputePipelines(vk::Device::get()->logicalDevice(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &_handle) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create compute pipeline!");
        }

        vkDestroyShaderModule( vk::Device::get()->logicalDevice(), shader_module, nullptr );
        return result;
    }

    void ComputeShader::despatch( uint32_t num_groups_x, uint32_t num_groups_y, uint32_t num_groups_z )
    {
        _command->dispatch( num_groups_x, num_groups_y, num_groups_z );
    }
    
    void ComputeShader::destruct()
    {
        vk::Shader::destruct();
    }

    ComputeShader::~ ComputeShader()
    {
        destruct();
    }

    ComputeShader::ComputeShader()
    :   kege::vk::Shader()
    {}

}}
