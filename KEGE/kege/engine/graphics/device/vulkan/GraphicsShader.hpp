//
//  GraphicsShader.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/4/23.
//

#ifndef KEGE_VULKAN_GRAPHICS_SHADER_HPP
#define KEGE_VULKAN_GRAPHICS_SHADER_HPP

#include "../../device/vulkan/RenderPass.hpp"
#include "../../device/vulkan/Shader.hpp"

namespace kege{namespace vk{

    class Device;
  
    class GraphicsShader : public vk::Shader
    {
    public:
        
        VkResult construct( const core::Shader::Info& info, const core::Renderpass* renderpass );
        void destruct();

        ~ GraphicsShader();
        GraphicsShader();

    private:

        void constructShaderPipelineLayout();

    private:

        VkPrimitiveTopology _topology;
        VkPolygonMode _polygonmode;
        VkRenderPass _renderpass;
    };
}}
#endif /* KEGE_VULKAN_GRAPHICS_SHADER_HPP */
