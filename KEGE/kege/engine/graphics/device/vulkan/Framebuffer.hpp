//
//  Framebuffer.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/11/23.
//

#ifndef KEGE_VULKAN_FRAMEBUFFER
#define KEGE_VULKAN_FRAMEBUFFER

#include <array>
#include <vector>
#include "../../device/core/Framebuffer.hpp"
#include "../../device/vulkan/Texture.hpp"
#include "../../device/vulkan/RenderPass.hpp"

namespace kege{namespace vk{

    class Framebuffer : public kege::core::Framebuffer
    {
    public:

        struct Attachment
        {
            kege::Ref< core::Texture > _texture;
            core::FBOCompType _type;
        };

        bool construct( const kege::Ref< vk::Renderpass >& renderpass );
        void attach( core::FBOCompType type, const kege::Ref< core::Texture >& texture );
        bool construct();

        const VkClearValue& cleardepth()const;
        VkClearValue& cleardepth();

        const VkClearValue& clearcolor()const;
        VkClearValue& clearcolor();

        const vk::Renderpass* renderpass()const;
        vk::Renderpass* renderpass();


        const vk::Framebuffer* vulkan()const;
        vk::Framebuffer* vulkan();

        const VkFramebuffer handle()const;
        const VkExtent2D& extent()const;

        bool hasDepthAttachment()const;
        
        uint32_t height()const;
        uint32_t width()const;
        void destruct();

        ~Framebuffer();
        Framebuffer();

    private:

        std::vector< Framebuffer::Attachment > _attachments;
        kege::Ref< vk::Renderpass > _renderpass;
        VkFramebuffer _framebuffer;

        bool _has_depth_attachment;

        VkClearValue _clearcolor;
        VkClearValue _cleardepth;
        VkExtent2D _extent;
    };

}}
#endif /* Framebuffer_hpp */
