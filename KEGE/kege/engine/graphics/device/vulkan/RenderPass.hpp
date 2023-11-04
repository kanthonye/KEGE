//
//  RenderPass.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/4/23.
//

#ifndef KEGE_VULKAN_RENDERPASS
#define KEGE_VULKAN_RENDERPASS

#include "../../device/core/RenderPass.hpp"

namespace kege{namespace vk{

    class Renderpass : public core::Renderpass
    {
    public:

        uint32_t addColorAttachment
        (
         VkFormat format, VkSampleCountFlagBits samples, VkImageLayout initial_layout, VkImageLayout final_layout
        );

        uint32_t addDepthAttachment
        (
         VkFormat format, VkSampleCountFlagBits samples, VkImageLayout initial_layout, VkImageLayout final_layout
        );

        const vk::Renderpass* vulkan()const;
        vk::Renderpass* vulkan();

        const VkRenderPass handle()const;
        VkRenderPass handle();
        VkResult construct();
        void destruct();

        ~Renderpass();
        Renderpass();

    private:

        std::vector< VkAttachmentDescription > _attachment_descriptions;
        std::vector< VkAttachmentReference > _attachment_references;

        uint32_t _depth_attachment_index;
        VkRenderPass _handle;
    };

}}
#endif /* KEGE_VULKAN_RENDERPASS */
