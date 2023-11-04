//
//  ComputeShader.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/3/23.
//

#ifndef KEGE_VULKAN_COMPUTE_SHADER_HPP
#define KEGE_VULKAN_COMPUTE_SHADER_HPP

#include "../../device/vulkan/Descriptor.hpp"
#include "../../device/vulkan/Shader.hpp"
#include "../../device/vulkan/RenderPass.hpp"

namespace kege{namespace vk{

    class Device;
    struct PipelineLayoutTempInfo;

    class ComputeShader : public kege::vk::Shader
    {
    public:

        void despatch( uint32_t num_groups_x, uint32_t num_groups_y, uint32_t num_groups_z );
        VkResult construct( const core::Shader::Info& info );
        void destruct();
        ~ ComputeShader();
        ComputeShader();
    };

}}
#endif /* KEGE_VULKAN_COMPUTE_SHADER_HPP */
