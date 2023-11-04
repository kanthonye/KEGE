//
//  RenderPass.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/14/23.
//

#ifndef RenderPass_hpp
#define RenderPass_hpp

#include <stdio.h>
#include "../../device/core/Abstraction.h"
namespace kege{namespace vk{
    class Renderpass;
}}
namespace kege{namespace core{

    class Renderpass : public kege::RefCounter
    {
    public:

        virtual const vk::Renderpass* vulkan()const = 0;
        virtual vk::Renderpass* vulkan() = 0;

        KEGE_CORE_ABSTRACTION( Renderpass );
    };

}}

//namespace kege{namespace core{
//    class RenderPass : public kege::RefCounter
//    {
//    public:
//
//        enum ColorFormat
//        {
//            R8G8B8A8_UNORM, // 8 bits per channel (32 bits total) with normalized values in the range [0, 1].
//            R8G8B8A8_SRGB, // Same as VK_FORMAT_R8G8B8A8_UNORM, but the color values are interpreted as sRGB.
//            B8G8R8A8_UNORM, // Similar to VK_FORMAT_R8G8B8A8_UNORM, but with a different byte order (BGRA).
//            R16G16B16A16_SFLOAT, // 16 bits per channel (64 bits total) with floating-point values.
//            R32G32B32A32_SFLOAT,
//        };
//
//        enum DepthFormat
//        {
//            D16_UNORM,         // 16-bit depth component.
//            D32_SFLOAT,        // 32-bit floating-point depth component.
//            D24_UNORM_PACK32,  // 24-bit depth component and an 8-bit stencil component packed together.
//            D24_UNORM_S8_UINT, // 24-bit depth component and an 8-bit stencil
//        };
//
//        enum Sample
//        {
//            SPP_1_BIT,  // Represents a single sample per pixel, indicating no multisampling.
//            SPP_2_BIT,  // Represents two samples per pixel.
//            SPP_4_BIT,  // Represents four samples per pixel.
//            SPP_8_BIT,  // Represents eight samples per pixel.
//            SPP_16_BIT, // Represents sixteen samples per pixel.
//            SPP_32_BIT, // Represents thirty-two samples per pixel.
//            SPP_64_BIT, // Represents sixty-four samples per pixel.
//        };
//
//        enum BindPoint
//        {
//            PIPELINE_GRAPHICS,
//            /* This value indicates that the subpass is a graphics subpass.
//            Graphics subpasses are used for rendering operations involving
//            vertex shaders, fragment shaders, and other graphics pipeline
//            stages.*/
//
//            PIPELINE_COMPUTE,
//            /* This value indicates that the subpass is a compute subpass.
//            Compute subpasses are used for general-purpose GPU computations,
//            where the shader stage is a compute shader. */
//
//            PIPELINE_RAYTRACE,
//            /* This value indicates that the subpass is a compute subpass.
//            Compute subpasses are used for general-purpose GPU computations,
//            where the shader stage is a compute shader. */
//        };
//
//        enum PipelineStageFlagBits
//        {
//            PIPELINE_STAGE_NONE = 0,
//            PIPELINE_STAGE_TOP_OF_PIPE_BIT,
//            PIPELINE_STAGE_DRAW_INDIRECT_BIT,
//            PIPELINE_STAGE_VERTEX_INPUT_BIT,
//            PIPELINE_STAGE_VERTEX_SHADER_BIT,
//            PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT,
//            PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT,
//            PIPELINE_STAGE_GEOMETRY_SHADER_BIT,
//            PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
//            PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
//            PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
//            PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
//            PIPELINE_STAGE_COMPUTE_SHADER_BIT,
//            PIPELINE_STAGE_TRANSFER_BIT,
//            PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
//            PIPELINE_STAGE_HOST_BIT,
//            PIPELINE_STAGE_ALL_GRAPHICS_BIT,
//            PIPELINE_STAGE_ALL_COMMANDS_BIT,
//        };
//
//        enum AccessFlagBits
//        {
//            ACCESS_NONE,
//            ACCESS_INDIRECT_COMMAND_READ_BIT,
//            ACCESS_INDEX_READ_BIT,
//            ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
//            ACCESS_UNIFORM_READ_BIT,
//            ACCESS_INPUT_ATTACHMENT_READ_BIT,
//            ACCESS_SHADER_READ_BIT,
//            ACCESS_SHADER_WRITE_BIT,
//            ACCESS_COLOR_ATTACHMENT_READ_BIT,
//            ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
//            ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
//            ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
//            ACCESS_TRANSFER_READ_BIT,
//            ACCESS_TRANSFER_WRITE_BIT,
//            ACCESS_HOST_READ_BIT,
//            ACCESS_HOST_WRITE_BIT,
//            ACCESS_MEMORY_READ_BIT,
//            ACCESS_MEMORY_WRITE_BIT,
//        };
//
//    public:
//
////        virtual void addSubpassDependency
////        (
////            uint32_t src_subpass,
////            uint32_t dst_subpass,
////            const std::vector< RenderPass::PipelineStageFlagBits >& src_stage_mask,
////            const std::vector< RenderPass::PipelineStageFlagBits >& dst_stage_mask,
////            const std::vector< RenderPass::AccessFlagBits >& src_access_mask,
////            const std::vector< RenderPass::AccessFlagBits >& dst_access_mask
////            //uint32_t dependency_flags
////        ) = 0;
////
////        virtual void addSubpass
////        (
////            RenderPass::BindPoint bindPoint,
////            const std::vector< uint32_t >& color_attachment_indices,
////            uint32_t depth_attachment_index = 0
////        ) = 0;
////
////        virtual uint32_t addColorAttachment
////        (
////            RenderPass::ColorFormat format,
////            RenderPass::Sample samples
////        ) = 0;
////
////        virtual uint32_t addDepthAttachment
////        (
////            RenderPass::DepthFormat format,
////            RenderPass::Sample samples
////        ) = 0;
//
//        virtual const vk::RenderPass* vulkan()const = 0;
//        virtual vk::RenderPass* vulkan() = 0;
//
////        virtual bool create() = 0;
//
//    protected:
//
//        RenderPass& operator =( const RenderPass& ) = delete;
//        RenderPass( const RenderPass& ) = delete;
//        virtual ~RenderPass(){}
//        RenderPass(){}
//    };
//}}
#endif /* RenderPass_hpp */
