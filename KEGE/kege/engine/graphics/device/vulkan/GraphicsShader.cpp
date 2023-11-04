//
//  GraphicsShader.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/4/23.
//

#include "../vulkan/utils.hpp"
#include "../vulkan/Device.hpp"
#include "../vulkan/GraphicsShader.hpp"
namespace kege{namespace vk{

    VkResult GraphicsShader::construct
    (
        const core::Shader::Info& info,
        const core::Renderpass* renderpass
    )
    {
        _name = info._name;
        _polygonmode = getPolygonMode( info._polygon_mode );
        _topology = getPrimitiveTopology( info._primitive_topology );
        
        std::vector< std::pair< VkShaderStageFlagBits, VkShaderModule > > pipeline_stages;
        std::vector< VkVertexInputAttributeDescription > vertex_input_attributes;
        std::vector< VkVertexInputBindingDescription > vertex_input_bindings;
        {
            std::vector< std::pair< uint32_t, std::vector<VkDescriptorSetLayoutBinding> > > descriptor_set_layout_bindings;
            std::vector< kege::Ref< vk::DescriptorSetLayout > > descriptor_set_layouts;
            std::vector< VkPushConstantRange > push_constant_ranges;
            vk::PipelineLayoutManager* mgr = vk::Device::get()->getPipelineLayoutManager();

            // CREATE SHADER MODULES & COLLECT SHADER ATTRIBUTES & UNIFORMS
            for ( const auto& stage : info._shader_stages )
            {
                std::vector< char > source;
                if( !loadShaderSource( source, stage.second.c_str() ) )
                {
                    return VK_INCOMPLETE;
                }

                VkShaderStageFlagBits shader_type = VK_SHADER_STAGE_VERTEX_BIT;
                switch ( stage.first )
                {
                    case core::Shader::VERTEX: shader_type = VK_SHADER_STAGE_VERTEX_BIT; break;
                    case core::Shader::FRAGMENT: shader_type = VK_SHADER_STAGE_FRAGMENT_BIT; break;
                    case core::Shader::TESS_CONTROL: shader_type = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT; break;
                    case core::Shader::TESS_EVALUATION: shader_type = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT; break;
                    case core::Shader::GEOMETRY: shader_type = VK_SHADER_STAGE_GEOMETRY_BIT; break;
                    default: KEGE_FATAL("UNKNOWN shader stage"); break;
                }

                /* 
                 from the shader source collect shader attributes, descriptiors and push-constants
                 */
                {
                    SpvReflectShaderModule reflect_module;
                    SpvReflectResult result = spvReflectCreateShaderModule( source.size(), source.data(), &reflect_module );
                    if (result != SPV_REFLECT_RESULT_SUCCESS)
                    {
                        throw std::runtime_error( "Error creating shader module" );
                    }

                    /*
                     collect attributes
                     */
                    parseVertexInputAttributes( shader_type, reflect_module, vertex_input_attributes, vertex_input_bindings );

                    /*
                     collect descriptiors
                     */
                    std::vector< kege::Ref< vk::DescriptorSetLayout > > a = mgr->createDescriptorSetLayouts( shader_type, reflect_module );
                    descriptor_set_layouts.insert( descriptor_set_layouts.end(), a.begin(), a.end() );

                    /*
                     collect push-constants
                     */
                    parsePushConstantRange( shader_type, reflect_module, push_constant_ranges );

                    /*
                     clean up
                     */
                    spvReflectDestroyShaderModule( &reflect_module );
                }

                /*
                 create shader module
                 */
                VkShaderModule shader_module;
                VkShaderModuleCreateInfo info{};
                info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                info.pCode = reinterpret_cast<const uint32_t*>( source.data() );
                info.codeSize = source.size();
                VkResult result = vkCreateShaderModule( vk::Device::get()->logicalDevice(), &info, nullptr, &shader_module );
                if ( result != VK_SUCCESS )
                {
                    KEGE_ERROR( vkGetError( result ) );
                    return result;
                }

                /*
                 store shader module
                 */
                pipeline_stages.push_back({ shader_type, shader_module });
            }

            // CREATE PIPELINE LAYOUT
            _pipeline_layout = mgr->createPipelineLayout( VK_PIPELINE_BIND_POINT_GRAPHICS, descriptor_set_layouts, push_constant_ranges );
        }

        VkGraphicsPipelineCreateInfo pipeline_info{};

        _renderpass = renderpass->vulkan()->handle();
        pipeline_info.renderPass = _renderpass;

        // SHADER INPUT ATTRIBUTE
        VkPipelineVertexInputStateCreateInfo vertex_input_info = {};
        vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        if ( !vertex_input_attributes.empty() )
        {
            vertex_input_info.vertexBindingDescriptionCount = static_cast< uint32_t >( vertex_input_bindings.size() );
            vertex_input_info.pVertexBindingDescriptions = vertex_input_bindings.data();
            vertex_input_info.vertexAttributeDescriptionCount = static_cast< uint32_t >( vertex_input_attributes.size() );
            vertex_input_info.pVertexAttributeDescriptions = vertex_input_attributes.data();
        }
        else
        {
            vertex_input_info.vertexBindingDescriptionCount = 0;
            vertex_input_info.vertexAttributeDescriptionCount = 0;
        }
        /* IMPORTANT: set -> pVertexInputState of VkGraphicsPipelineCreateInfo */
        pipeline_info.pVertexInputState = &vertex_input_info;


        // SETTING THE SHADER CREATE STAGE INFO
        std::vector< VkPipelineShaderStageCreateInfo > shader_stage_create_infos;
        for (auto& m : pipeline_stages )
        {
            VkPipelineShaderStageCreateInfo create_info{};
            create_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            create_info.module = m.second;
            create_info.stage  = m.first;
            create_info.pName  = "main";
            shader_stage_create_infos.push_back( create_info );
        }

        VkPipelineInputAssemblyStateCreateInfo input_assembly{};
        input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_assembly.topology = _topology;
        input_assembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewport_state{};
        viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_state.viewportCount = 1;
        viewport_state.scissorCount = 1;

        VkPipelineMultisampleStateCreateInfo multisampling = {VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f;
        multisampling.pSampleMask = 0;
        multisampling.alphaToCoverageEnable = VK_FALSE;
        multisampling.alphaToOneEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState color_blend_attachment{};
        color_blend_attachment.blendEnable = VK_TRUE;
        color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
        color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;
        color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        color_blend_attachment.colorWriteMask =
        {
            VK_COLOR_COMPONENT_R_BIT |
            VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT
        };

        VkPipelineColorBlendStateCreateInfo color_blending{};
        color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blending.logicOpEnable = VK_FALSE;
        color_blending.logicOp = VK_LOGIC_OP_COPY;
        color_blending.attachmentCount = 1;
        color_blending.pAttachments = &color_blend_attachment;
        color_blending.blendConstants[0] = 0.0f;
        color_blending.blendConstants[1] = 0.0f;
        color_blending.blendConstants[2] = 0.0f;
        color_blending.blendConstants[3] = 0.0f;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = _polygonmode;
        rasterizer.lineWidth = 1.0f;
        //rasterizer.cullMode = VK_CULL_MODE_NONE;
        //rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        if ( info._cull_mode == core::CullMode::CW )
        {
            rasterizer.cullMode = VK_CULL_MODE_FRONT_BIT;
            rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        }
        else if ( info._cull_mode == core::CullMode::CCW )
        {
            rasterizer.cullMode = VK_CULL_MODE_FRONT_BIT;
            rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        }
        else if ( info._cull_mode == core::CullMode::CW_CCW )
        {
            rasterizer.cullMode = VK_CULL_MODE_NONE;
            rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        }

        VkPipelineDepthStencilStateCreateInfo depth_stencil{};
        depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depth_stencil.depthTestEnable = VK_TRUE;
        depth_stencil.depthWriteEnable = VK_TRUE;
        depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
        depth_stencil.depthBoundsTestEnable = VK_FALSE;
        depth_stencil.minDepthBounds = 0.0f; // Optional
        depth_stencil.maxDepthBounds = 1.0f; // Optional
        depth_stencil.stencilTestEnable = VK_FALSE;
        depth_stencil.front = {}; // Optional
        depth_stencil.back = {}; // Optional

        std::vector< VkDynamicState > dynamic_states =
        {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
            VK_DYNAMIC_STATE_LINE_WIDTH
        };

        VkPipelineDynamicStateCreateInfo dynamic_state{};
        dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state.dynamicStateCount = static_cast< uint32_t >( dynamic_states.size() );
        dynamic_state.pDynamicStates = dynamic_states.data();

        pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipeline_info.stageCount = (uint32_t) shader_stage_create_infos.size();
        pipeline_info.pStages = shader_stage_create_infos.data();
        pipeline_info.pInputAssemblyState = &input_assembly;
        pipeline_info.pViewportState = &viewport_state;
        pipeline_info.pRasterizationState = &rasterizer;
        pipeline_info.pMultisampleState = &multisampling;
        pipeline_info.pColorBlendState = &color_blending;
        pipeline_info.pDynamicState = &dynamic_state;
        pipeline_info.layout = _pipeline_layout->handle();
        pipeline_info.subpass = 0;
        pipeline_info.pDepthStencilState = &depth_stencil;
        pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

        // CREATE THE VULKAN GRAPHICS PIPELINE

        VkResult result = vkCreateGraphicsPipelines( vk::Device::get()->logicalDevice(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &_handle );
        if ( result != VK_SUCCESS )
        {
            for (auto& ss : pipeline_stages )
            {
                vkDestroyShaderModule( vk::Device::get()->logicalDevice(), ss.second, nullptr );
            }
            KEGE_ERROR( "%s: vkCreateGraphicsPipelines()", vkGetError( result ) );
            return result;
        }
        
        for (auto& ss : pipeline_stages )
        {
            vkDestroyShaderModule( vk::Device::get()->logicalDevice(), ss.second, nullptr );
        }
        pipeline_stages.clear();
        return result;
    }

    void GraphicsShader::destruct()
    {
        vk::Shader::destruct();
    }

    GraphicsShader::~ GraphicsShader()
    {
        destruct();
    }

    GraphicsShader::GraphicsShader()
    :   _topology( VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST )
    ,   _polygonmode( VK_POLYGON_MODE_FILL )
    ,   _renderpass( VK_NULL_HANDLE )
    {}

}}
