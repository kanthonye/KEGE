//
//  Framebuffer.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/11/23.
//

#include "../../device/debug/kege-logger.hpp"
#include "../../device/vulkan/Device.hpp"
#include "../../device/vulkan/Image.hpp"
#include "../../device/vulkan/RenderPass.hpp"
#include "../../device/vulkan/Framebuffer.hpp"

namespace kege{namespace vk{

    void Framebuffer::attach( core::FBOCompType type, const kege::Ref< core::Texture >& texture )
    {
        _extent = { texture->width(),  texture->height() };
        _attachments.push_back({ texture, type });
        if( type == core::DEPTH_COMPONENT ) _has_depth_attachment = true;
    }

    bool Framebuffer::construct( const kege::Ref< vk::Renderpass >& renderpass )
    {
        std::vector< VkImageView > imageviews;
        for (auto& attachment : _attachments) imageviews.push_back( attachment._texture->vulkan()->image().view().handle() );

        VkFramebufferCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        info.attachmentCount = static_cast<uint32_t>( imageviews.size() );
        info.pAttachments = imageviews.data();
        info.renderPass = renderpass->handle();
        info.width  = _attachments[0]._texture->width();
        info.height = _attachments[0]._texture->height();
        info.layers = 1;

        VkResult result = vkCreateFramebuffer( vk::Device::get()->logicalDevice(), &info, nullptr, &_framebuffer );
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR( "$s : Framebuffer creatation failed.", vkGetError( result ) );
            return false;
        }
        _renderpass = renderpass;
        return true;
    }

    const VkClearValue& Framebuffer::cleardepth()const
    {
        return _cleardepth;
    }

    VkClearValue& Framebuffer::cleardepth()
    {
        return _cleardepth;
    }

    const VkClearValue& Framebuffer::clearcolor()const
    {
        return _clearcolor;
    }

    VkClearValue& Framebuffer::clearcolor()
    {
        return _clearcolor;
    }

    const vk::Renderpass* Framebuffer::renderpass()const
    {
        return _renderpass.ref();
    }

    vk::Renderpass* Framebuffer::renderpass()
    {
        return _renderpass.ref();
    }

    bool Framebuffer::construct()
    {
        kege::Ref< vk::Renderpass > renderpass = new vk::Renderpass();
        for (auto& attachment : _attachments)
        {
            VkFormat format = attachment._texture->vulkan()->image().format();
            switch ( attachment._type )
            {
                case core::FBOCompType::COLOR_COMPONENT:
                {
                    renderpass->addColorAttachment( format, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
                    break;
                }

                case core::FBOCompType::DEPTH_COMPONENT:
                {
                    renderpass->addDepthAttachment( format, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
                    _has_depth_attachment = true;
                    break;
                }

                default: break;
            }
        }
        VkResult result = renderpass->construct();
        if( result != VK_SUCCESS )
        {
            KEGE_ERROR( "%s: Framebuffer creatation failed. Something went wrong with Renderpass creation.", vkGetError( result ) );
            return false;
        }
        return construct( renderpass );
    }

    const vk::Framebuffer* Framebuffer::vulkan()const
    {
        return this;
    }
    vk::Framebuffer* Framebuffer::vulkan()
    {
        return this;
    }

    const VkFramebuffer Framebuffer::handle()const
    {
        return _framebuffer;
    }
    const VkExtent2D& Framebuffer::extent()const
    {
        return _extent;
    }

    bool Framebuffer::hasDepthAttachment()const
    {
        return _has_depth_attachment;
    }

    uint32_t Framebuffer::height()const
    {
        return _extent.height;
    }

    uint32_t Framebuffer::width()const
    {
        return _extent.width;
    }

    void Framebuffer::destruct()
    {
        _attachments.clear();
        _renderpass.clear();
        
        if ( _framebuffer )
        {
            vk::Device::get()->waitIdle();
            vkDestroyFramebuffer( vk::Device::get()->logicalDevice(), _framebuffer, nullptr );
            _framebuffer = VK_NULL_HANDLE;
        }
    }
    
    Framebuffer::~Framebuffer()
    {
        destruct();
    }

    Framebuffer::Framebuffer()
    :   _framebuffer( VK_NULL_HANDLE )
    ,   _extent({})
    ,   _has_depth_attachment( false )
    {
        _clearcolor.color = {{0.02, 0.02, 0.0225, 1.0}};
        _cleardepth.depthStencil = {1.0f, 0};
        //_clear_values[0].color = {{0.2, 0.2, 0.225, 1.0}};
        //_clear_values[1].depthStencil = {1.0f, 0};
    }
}}
