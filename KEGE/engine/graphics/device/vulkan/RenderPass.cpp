//
//  RenderPass.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/4/23.
//

#include "../../device/vulkan/Device.hpp"
#include "../../device/vulkan/RenderPass.hpp"
namespace kege{namespace vk{

    uint32_t Renderpass::addColorAttachment
    (
        VkFormat format, VkSampleCountFlagBits samples, VkImageLayout initial_layout, VkImageLayout final_layout
    )
    {
        VkAttachmentDescription description = {};
        description.format = format;
        description.samples = samples;

        /*
         VK_ATTACHMENT_LOAD_OP_LOAD: This option loads the existing contents of the
         attachment. It's useful when you want to preserve the previous content and
         continue rendering on top of it. For example, if you're doing incremental
         rendering and want to accumulate results.

         VK_ATTACHMENT_LOAD_OP_DONT_CARE: This option indicates that the previous
         contents of the attachment are not relevant. Vulkan does not guarantee the
         preservation or clearing of the attachment's contents. This can be useful
         for performance optimization if you don't need the previous contents.

         VK_ATTACHMENT_LOAD_OP_CLEAR: As you mentioned earlier, this option clears
         the attachment's contents at the beginning of the render pass. It's
         typically used when you want a clean slate for rendering.
         */
        description.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        
        description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;  // Specify the store operation (e.g., store the attachment)
        description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;  // Specify the stencil load operation (if applicable)
        description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;  // Specify the stencil store operation (if applicable)
        description.initialLayout = initial_layout;  //VK_IMAGE_LAYOUT_UNDEFINED;  // Specify the initial layout of the attachment
        description.finalLayout = final_layout; //VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;  // Specify the final layout of the attachment

        VkAttachmentReference reference{};
        reference.attachment = static_cast<uint32_t>( _attachment_descriptions.size() );
        reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        _attachment_descriptions.push_back( description );
        _attachment_references.push_back( reference );

        return reference.attachment;
    }

    uint32_t Renderpass::addDepthAttachment
    (
        VkFormat format, VkSampleCountFlagBits samples, VkImageLayout initial_layout, VkImageLayout final_layout
    )
    {
        VkAttachmentDescription description = {};
        description.format = format;
        description.samples = samples;
        description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        description.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        description.initialLayout = initial_layout;//VK_IMAGE_LAYOUT_UNDEFINED;
        description.finalLayout = final_layout;//VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference reference = {};
        reference.attachment = static_cast<uint32_t>( _attachment_descriptions.size() );
        reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        _depth_attachment_index = static_cast<uint32_t>( _attachment_descriptions.size() );
        _attachment_descriptions.push_back( description );
        _attachment_references.push_back( reference );

        return reference.attachment;
    }

    const vk::Renderpass* Renderpass::vulkan()const
    {
        return this;
    }

    vk::Renderpass* Renderpass::vulkan()
    {
        return this;
    }

    VkResult Renderpass::construct()
    {
        std::vector< VkAttachmentReference > color_references = _attachment_references;
        std::swap(color_references[ color_references.size() - 1 ], color_references[ _depth_attachment_index ] );
        color_references.pop_back();

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.pDepthStencilAttachment = &_attachment_references[ _depth_attachment_index ];
        subpass.colorAttachmentCount = static_cast<uint32_t>( color_references.size() );
        subpass.pColorAttachments = color_references.data();

        // Use subpass dependencies for layout transitions
        std::array<VkSubpassDependency, 2> dependencies;

        dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
        dependencies[0].dstSubpass = 0;
        dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

        dependencies[1].srcSubpass = 0;
        dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
        dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;


//        VkSubpassDependency dependency{};
//        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
//        dependency.dstSubpass = 0;
//        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
//        dependency.srcAccessMask = 0;
//        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
//        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;


        VkRenderPassCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

        info.attachmentCount = static_cast<uint32_t>( _attachment_descriptions.size() );
        info.pAttachments = _attachment_descriptions.data();

        info.subpassCount = 1;
        info.pSubpasses = &subpass;

        info.dependencyCount = static_cast< uint32_t >( dependencies.size() );
        info.pDependencies = dependencies.data();

        return vkCreateRenderPass( vk::Device::get()->logicalDevice(), &info, nullptr, &_handle );
    }

    const VkRenderPass Renderpass::handle()const
    {
        return _handle;
    }

    VkRenderPass Renderpass::handle()
    {
        return _handle;
    }

    void Renderpass::destruct()
    {
        _attachment_references.clear();
        _attachment_descriptions.clear();

        if( _handle != VK_NULL_HANDLE )
        {
            vk::Device::get()->waitIdle();
            vkDestroyRenderPass( vk::Device::get()->logicalDevice(), _handle, nullptr );
            _handle = VK_NULL_HANDLE;
        }
    }

    Renderpass::~Renderpass()
    {
        destruct();
    }

    Renderpass::Renderpass()
    :   _attachment_descriptions( 0 )
    ,   _handle( VK_NULL_HANDLE )
    {}
}}

//namespace kege{namespace vk{
//
//    VkPipelineStageFlags toVkPipelineStageFlags( const std::vector< RenderPass::PipelineStageFlagBits >& flags );
//    VkPipelineStageFlagBits toVkPipelineStageFlagBits( RenderPass::PipelineStageFlagBits flag );
//
//    VkAccessFlags toAccessFlags( const std::vector< RenderPass::AccessFlagBits >& flags );
//    VkAccessFlagBits toVkAccessFlagBits( RenderPass::AccessFlagBits flag );
//
//    VkSampleCountFlagBits toSampleCountFlagBits( RenderPass::Sample samples );
//
//    VkFormat toVkFormat( RenderPass::ColorFormat color_format );
//    VkFormat toVkFormat( RenderPass::DepthFormat depth_format );
//    VkPipelineBindPoint toVkPipelineBindPoint( RenderPass::BindPoint bind_point );
//
//    void RenderPass::addSubpassDependency
//    (
//        uint32_t src_subpass,
//        uint32_t dst_subpass,
//        const std::vector< RenderPass::PipelineStageFlagBits >& src_stage_mask,
//        const std::vector< RenderPass::PipelineStageFlagBits >& dst_stage_mask,
//        const std::vector< RenderPass::AccessFlagBits >& src_access_mask,
//        const std::vector< RenderPass::AccessFlagBits >& dst_access_mask
//        //uint32_t dependency_flags
//    )
//    {
//        addSubpassDependency
//        (
//            src_subpass, dst_subpass,
//            toVkPipelineStageFlags( src_stage_mask ),
//            toVkPipelineStageFlags( dst_stage_mask ),
//            toAccessFlags( src_access_mask ),
//            toAccessFlags( dst_access_mask )
//        );
//    }
//
//    void RenderPass::addSubpass
//    (
//        RenderPass::BindPoint bind_point,
//        const std::vector< uint32_t >& color_attachment_indices,
//        uint32_t depth_attachment_index
//    )
//    {
//        addSubpass
//        (
//            toVkPipelineBindPoint( bind_point ),
//            color_attachment_indices,
//            depth_attachment_index
//        );
//    }
//
//    uint32_t RenderPass::addColorAttachment
//    (
//        RenderPass::ColorFormat format,
//        RenderPass::Sample samples
//    )
//    {
//        return addColorAttachment( toVkFormat( format ), toSampleCountFlagBits( samples ) );
//    }
//
//    uint32_t RenderPass::addDepthAttachment
//    (
//        RenderPass::DepthFormat format,
//        RenderPass::Sample samples
//    )
//    {
//        return addDepthAttachment( toVkFormat( format ), toSampleCountFlagBits( samples ) );
//    }
//
//    void RenderPass::addSubpassDependency
//    (
//        uint32_t src_subpass,
//        uint32_t dst_subpass,
//        VkPipelineStageFlags src_stage_mask,
//        VkPipelineStageFlags dst_stage_mask,
//        VkAccessFlags src_access_mask,
//        VkAccessFlags dst_access_mask
//    )
//    {
//        VkSubpassDependency dependency = {};
//        dependency.srcSubpass = src_subpass;
//        dependency.dstSubpass = dst_subpass;
//        dependency.srcStageMask = src_stage_mask;
//        dependency.dstStageMask = dst_stage_mask;
//        dependency.srcAccessMask = src_access_mask;
//        dependency.dstAccessMask = dst_access_mask;
//        //dependency.dependencyFlags = dependency_flags;
//        _subpass_dependencies.push_back(dependency);
//    }
//
//    void RenderPass::addSubpass
//    (
//        VkPipelineBindPoint bind_point,
//        const std::vector< uint32_t >& color_attachment_indices,
//        int32_t depth_attachment_index
//    )
//    {
//        int n = 0;
//        VkAttachmentReference* references = new VkAttachmentReference[ color_attachment_indices.size() ];
//        for (const uint32_t& i : color_attachment_indices ) references[n++] = _attachment_references[i];
//
//        VkSubpassDescription description = {};
//        description.pipelineBindPoint = bind_point;
//        description.pColorAttachments = references;
//        description.colorAttachmentCount = static_cast<uint32_t>( color_attachment_indices.size() );
//        description.pDepthStencilAttachment = (depth_attachment_index >= 0) ? &_attachment_references[ depth_attachment_index ] : nullptr;
//
//        _subpass_descriptions.push_back( description );
//    }
//
//    uint32_t RenderPass::addColorAttachment
//    (
//        VkFormat format,
//        VkSampleCountFlagBits samples
//    )
//    {
//        VkAttachmentDescription description = {};
//        description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;  // Specify the load operation (e.g., clear the attachment)
//        description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;  // Specify the store operation (e.g., store the attachment)
//        description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;  // Specify the stencil load operation (if applicable)
//        description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;  // Specify the stencil store operation (if applicable)
//        description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;  // Specify the initial layout of the attachment
//        description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;  // Specify the final layout of the attachment
//        description.samples = samples;
//        description.format = format;
//
//        VkAttachmentReference reference{};
//        reference.attachment = static_cast<uint32_t>( _attachment_descriptions.size() );
//        reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//
//        _attachment_descriptions.push_back( description );
//        _attachment_references.push_back( reference );
//
//        return reference.attachment;
//    }
//
//    uint32_t RenderPass::addDepthAttachment
//    (
//        VkFormat format,
//        VkSampleCountFlagBits samples
//    )
//    {
//        VkAttachmentDescription description = {};
//        description.format = format;
//        description.samples = samples;
//        description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//        description.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//        description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//        description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//        description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//        description.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
//
//        VkAttachmentReference reference = {};
//        reference.attachment = static_cast<uint32_t>( _attachment_descriptions.size() );
//        reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
//
//        _depth_attachment_index = static_cast<uint32_t>( _attachment_descriptions.size() );
//        _attachment_descriptions.push_back( description );
//        _attachment_references.push_back( reference );
//
//        return reference.attachment;
//    }
//
//    void RenderPass::destruct()
//    {
//        for (int i = 0; i < _subpass_descriptions.size(); i++)
//        {
//            delete _subpass_descriptions[i].pColorAttachments;
//        }
//
//        _subpass_descriptions.clear();
//        _subpass_dependencies.clear();
//        _attachment_references.clear();
//        _attachment_descriptions.clear();
//
//        if( _handle != VK_NULL_HANDLE )
//        {
//            _device->waitIdle();
//            vkDestroyRenderPass( _device->device(), _handle, nullptr );
//            _handle = VK_NULL_HANDLE;
//        }
//    }
//
//    bool RenderPass::construct( vk::Device* device )
//    {
//        VkRenderPassCreateInfo info{};
//        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
//
//        info.attachmentCount = static_cast<uint32_t>( _attachment_descriptions.size() );
//        info.pAttachments = _attachment_descriptions.data();
//
//        info.subpassCount = static_cast<uint32_t>( _subpass_descriptions.size() );
//        info.pSubpasses = _subpass_descriptions.data();
//
//        info.dependencyCount = static_cast<uint32_t>( _subpass_dependencies.size() );
//        info.pDependencies = _subpass_dependencies.data();
//
//        if ( vkCreateRenderPass( device->device(), &info, nullptr, &_handle ) != VK_SUCCESS )
//        {
//            throw std::runtime_error("failed to create render pass!");
//        }
//        _device = device;
//        return true;
//    }
//
//    VkSampleCountFlagBits toSampleCountFlagBits( RenderPass::Sample samples )
//    {
//        switch ( samples )
//        {
//            case RenderPass::SPP_1_BIT:  return VK_SAMPLE_COUNT_1_BIT;
//            // Represents a single sample per pixel, indicating no multisampling.
//
//            case RenderPass::SPP_2_BIT:  return VK_SAMPLE_COUNT_2_BIT;
//            // Represents two samples per pixel.
//
//            case RenderPass::SPP_4_BIT:  return VK_SAMPLE_COUNT_4_BIT;
//            // Represents four samples per pixel.
//
//            case RenderPass::SPP_8_BIT:  return VK_SAMPLE_COUNT_8_BIT;
//            // Represents eight samples per pixel.
//
//            case RenderPass::SPP_16_BIT: return VK_SAMPLE_COUNT_16_BIT;
//            // Represents sixteen samples per pixel.
//
//            case RenderPass::SPP_32_BIT: return VK_SAMPLE_COUNT_32_BIT;
//            // Represents thirty-two samples per pixel.
//
//            case RenderPass::SPP_64_BIT: return VK_SAMPLE_COUNT_64_BIT;
//            // Represents sixty-four samples per pixel.
//        }
//    };
//
//    VkPipelineStageFlagBits toVkPipelineStageFlagBits( RenderPass::PipelineStageFlagBits flag )
//    {
//        switch( flag )
//        {
//            case RenderPass::PIPELINE_STAGE_TOP_OF_PIPE_BIT:
//                return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
//
//            case RenderPass::PIPELINE_STAGE_DRAW_INDIRECT_BIT:
//                return VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
//
//            case RenderPass::PIPELINE_STAGE_VERTEX_INPUT_BIT:
//                return VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
//
//            case RenderPass::PIPELINE_STAGE_VERTEX_SHADER_BIT:
//                return VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
//
//            case RenderPass::PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT:
//                return VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
//
//            case RenderPass::PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT:
//                return VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
//
//            case RenderPass::PIPELINE_STAGE_GEOMETRY_SHADER_BIT:
//                return VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
//
//            case RenderPass::PIPELINE_STAGE_FRAGMENT_SHADER_BIT:
//                return VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
//
//            case RenderPass::PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT:
//                return VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
//
//            case RenderPass::PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT:
//                return VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
//
//            case RenderPass::PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT:
//                return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//
//            case RenderPass::PIPELINE_STAGE_COMPUTE_SHADER_BIT:
//                return VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
//
//            case RenderPass::PIPELINE_STAGE_TRANSFER_BIT:
//                return VK_PIPELINE_STAGE_TRANSFER_BIT;
//
//            case RenderPass::PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT:
//                return VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
//
//            case RenderPass::PIPELINE_STAGE_HOST_BIT:
//                return VK_PIPELINE_STAGE_HOST_BIT;
//
//            case RenderPass::PIPELINE_STAGE_ALL_GRAPHICS_BIT:
//                return VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
//
//            case RenderPass::PIPELINE_STAGE_ALL_COMMANDS_BIT:
//                return VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
//
//            case RenderPass::PIPELINE_STAGE_NONE:
//                return VK_PIPELINE_STAGE_NONE;
//        }
//    }
//
//    VkPipelineStageFlags toVkPipelineStageFlags( const std::vector< RenderPass::PipelineStageFlagBits >& flags )
//    {
//        VkPipelineStageFlags bitmask = VK_PIPELINE_STAGE_NONE;
//        for ( const RenderPass::PipelineStageFlagBits& f : flags )
//        {
//            bitmask |= toVkPipelineStageFlagBits( f );
//        }
//        return bitmask;
//    }
//
//    VkAccessFlagBits toVkAccessFlagBits( RenderPass::AccessFlagBits flag )
//    {
//        switch( flag )
//        {
//            case RenderPass::ACCESS_INDIRECT_COMMAND_READ_BIT:
//                return VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
//
//            case RenderPass::ACCESS_INDEX_READ_BIT:
//                return VK_ACCESS_INDEX_READ_BIT;
//
//            case RenderPass::ACCESS_VERTEX_ATTRIBUTE_READ_BIT:
//                return VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
//
//            case RenderPass::ACCESS_UNIFORM_READ_BIT:
//                return VK_ACCESS_UNIFORM_READ_BIT;
//
//            case RenderPass::ACCESS_INPUT_ATTACHMENT_READ_BIT:
//                return VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
//
//            case RenderPass::ACCESS_SHADER_READ_BIT:
//                return VK_ACCESS_SHADER_READ_BIT;
//
//            case RenderPass::ACCESS_SHADER_WRITE_BIT:
//                return VK_ACCESS_SHADER_WRITE_BIT;
//
//            case RenderPass::ACCESS_COLOR_ATTACHMENT_READ_BIT:
//                return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
//
//            case RenderPass::ACCESS_COLOR_ATTACHMENT_WRITE_BIT:
//                return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//
//            case RenderPass::ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT:
//                return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
//
//            case RenderPass::ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT:
//                return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
//
//            case RenderPass::ACCESS_TRANSFER_READ_BIT:
//                return VK_ACCESS_TRANSFER_READ_BIT;
//
//            case RenderPass::ACCESS_TRANSFER_WRITE_BIT:
//                return VK_ACCESS_TRANSFER_WRITE_BIT;
//
//            case RenderPass::ACCESS_HOST_READ_BIT:
//                return VK_ACCESS_HOST_READ_BIT;
//
//            case RenderPass::ACCESS_HOST_WRITE_BIT:
//                return VK_ACCESS_HOST_WRITE_BIT;
//
//            case RenderPass::ACCESS_MEMORY_READ_BIT:
//                return VK_ACCESS_MEMORY_READ_BIT;
//
//            case RenderPass::ACCESS_MEMORY_WRITE_BIT:
//                return VK_ACCESS_MEMORY_WRITE_BIT;
//
//            case RenderPass::ACCESS_NONE:
//                return VK_ACCESS_NONE;
//        }
//    }
//
//    VkAccessFlags toAccessFlags( const std::vector< RenderPass::AccessFlagBits >& flags )
//    {
//        VkAccessFlags bitmask = VK_PIPELINE_STAGE_NONE;
//        for ( const RenderPass::AccessFlagBits& f : flags )
//        {
//            bitmask |= toVkAccessFlagBits( f );
//        }
//        return bitmask;
//    }
//
//
//    VkFormat toVkFormat( RenderPass::ColorFormat color_format )
//    {
//        VkFormat format;
//        switch ( color_format )
//        {
//            case RenderPass::R8G8B8A8_UNORM: format = VK_FORMAT_R8G8B8A8_UNORM;
//                break;
//
//            case RenderPass::R8G8B8A8_SRGB: format = VK_FORMAT_R8G8B8A8_SRGB;
//                break;
//
//            case RenderPass::B8G8R8A8_UNORM: format = VK_FORMAT_B8G8R8A8_UNORM;
//                break;
//
//            case RenderPass::R16G16B16A16_SFLOAT: format = VK_FORMAT_R16G16B16A16_SFLOAT;
//                break;
//
//            case RenderPass::R32G32B32A32_SFLOAT: format = VK_FORMAT_R32G32B32A32_SFLOAT;
//                break;
//        }
//        return format;
//    }
//
//    VkFormat toVkFormat( RenderPass::DepthFormat depth_format )
//    {
//        VkFormat format;
//        switch ( depth_format )
//        {
//            case RenderPass::D16_UNORM: format = VK_FORMAT_D16_UNORM;
//                break;
//
//            case RenderPass::D32_SFLOAT: format = VK_FORMAT_D32_SFLOAT;
//                break;
//
//            case RenderPass::D24_UNORM_PACK32: format = VK_FORMAT_X8_D24_UNORM_PACK32;
//                break;
//
//            case RenderPass::D24_UNORM_S8_UINT: format = VK_FORMAT_D24_UNORM_S8_UINT;
//                break;
//        }
//        return format;
//    }
//
//    VkPipelineBindPoint toVkPipelineBindPoint( RenderPass::BindPoint bind_point )
//    {
//        VkPipelineBindPoint pipeline_bind_point;
//        switch( bind_point )
//        {
//            case RenderPass::PIPELINE_GRAPHICS: pipeline_bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS;
//            case RenderPass::PIPELINE_COMPUTE:  pipeline_bind_point = VK_PIPELINE_BIND_POINT_COMPUTE;
//            case RenderPass::PIPELINE_RAYTRACE: pipeline_bind_point = VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR;
//        }
//        return pipeline_bind_point;
//    }
//
//    const VkRenderPass RenderPass::handle()const
//    {
//        return _handle;
//    }
//    VkRenderPass RenderPass::handle()
//    {
//        return _handle;
//    }
//
//    bool RenderPass::create()
//    {
//        return false;
//    }
//
//    RenderPass::~RenderPass()
//    {
//        destruct();
//    }
//
//    RenderPass::RenderPass()
//    :   _handle( VK_NULL_HANDLE )
//    {
//    }
//}}
