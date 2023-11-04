//
//  Texture.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/21/23.
//

#include "../vulkan/Device.hpp"
#include "../vulkan/Buffer.hpp"
#include "../vulkan/Texture.hpp"

namespace kege{namespace vk{

    VkResult Texture::constructTexture2d
    (
        uint32_t width,
        uint32_t height,
        uint32_t mip_levels,
        core::ImgFormat format,
        const void* data,
        bool storage
    )
    {
        VkImageAspectFlags aspect_mask;

        VkImageCreateInfo image_info{};
        image_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
        image_info.samples       = VK_SAMPLE_COUNT_1_BIT;
        image_info.extent.width  = width;
        image_info.extent.height = height;
        image_info.extent.depth  = 1;
        image_info.mipLevels     = mip_levels;
        image_info.arrayLayers   = 1;
        image_info.format        = getVkFormat( format );
        image_info.imageType     = VK_IMAGE_TYPE_2D;
        image_info.tiling        = VK_IMAGE_TILING_OPTIMAL;
        image_info.flags         = 0; // Optional

        image_info.usage = ( storage ) ? VK_IMAGE_USAGE_STORAGE_BIT : 0; //;

        switch ( format )
        {
            case core::D16:
            case core::D24:
            case core::D32:
                image_info.usage |=
                {
                    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
                    VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                    VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                    VK_IMAGE_USAGE_SAMPLED_BIT
                };
                aspect_mask = VK_IMAGE_ASPECT_DEPTH_BIT;
                break;

            default:
                image_info.usage |=
                {
                    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                    VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                    VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                    VK_IMAGE_USAGE_SAMPLED_BIT
                };
                aspect_mask = VK_IMAGE_ASPECT_COLOR_BIT;
                break;
        }

        VkResult result = operator()
        (
            image_info,
            VK_IMAGE_VIEW_TYPE_2D,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            aspect_mask,
            data
        );

        return result;
    }

    VkResult Texture::constructTexture3d
    (
        uint32_t width,
        uint32_t height,
        uint32_t depth,
        uint32_t mip_levels,
        core::ImgFormat format,
        const void* data
    )
    {
        VkImageCreateInfo image_info{};
        image_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
        image_info.samples       = VK_SAMPLE_COUNT_1_BIT;
        image_info.extent.width  = width;
        image_info.extent.height = height;
        image_info.extent.depth  = depth;
        image_info.mipLevels     = mip_levels;
        image_info.arrayLayers   = 1;
        image_info.format        = getVkFormat( format );
        image_info.imageType     = VK_IMAGE_TYPE_3D;
        image_info.tiling        = VK_IMAGE_TILING_OPTIMAL;
        image_info.flags         = 0; // Optional
        image_info.usage         =
        {
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
            VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
            VK_IMAGE_USAGE_TRANSFER_DST_BIT |
            VK_IMAGE_USAGE_SAMPLED_BIT
        };

        VkResult result = operator()
        (
            image_info,
            VK_IMAGE_VIEW_TYPE_3D,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            VK_IMAGE_ASPECT_COLOR_BIT,
            data
        );
        
        return result;
    }

    VkResult Texture::operator()
    (
        const VkImageCreateInfo& create_info,
        VkImageViewType view_type,
        VkMemoryPropertyFlags memory_property,
        VkImageAspectFlags aspect_mask,
        const void* data
    )
    {
        VkResult result = _image.construct( create_info, view_type, memory_property, aspect_mask );
        if ( result != VK_SUCCESS )
        {
            KEGE_FATAL("%s : attempt to construct vk::Texture2d fail.", vkGetError( result ));
            return result;
        }
        if ( data )
        {
            vk::Buffer source;
            source.construct( core::Buffer::TRANSFER, create_info.extent.width * create_info.extent.height * 4, data, core::Buffer::STAGE );
            vk::Device::get()->copyBufferToImage( source, _image );
        }
        return result;
    }

    VkImageLayout getImageLayout( VkImage image )
    {
        VkSubresourceLayout subresourceLayout;
        VkImageSubresource subresource = {};
        subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        vkGetImageSubresourceLayout( vk::Device::get()->logicalDevice(), image, &subresource, &subresourceLayout );

        VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;

        // Determine the current layout based on the subresourceLayout
        if (subresource.aspectMask == VK_IMAGE_ASPECT_COLOR_BIT) 
        {
            layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Example layout for color images
        } 
        else if (subresource.aspectMask == VK_IMAGE_ASPECT_DEPTH_BIT)
        {
            layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; // Example layout for depth images
        }
        return layout;
    }

    void Texture::setImageLayout( VkCommandBuffer command, VkImageLayout new_layout )
    {
        VkImageMemoryBarrier image_barrier = {};
        image_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        image_barrier.pNext = NULL;
        image_barrier.oldLayout = _image._layout;
        image_barrier.newLayout = new_layout;
        image_barrier.image = _image._source.handle();
        image_barrier.subresourceRange.aspectMask = _image._aspect_flags;
        image_barrier.subresourceRange.baseMipLevel = 0;
        image_barrier.subresourceRange.levelCount = 1;
        image_barrier.subresourceRange.layerCount = 1;

        switch ( _image._layout )
        {
            case VK_IMAGE_LAYOUT_PREINITIALIZED:
                image_barrier.srcAccessMask =
                VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
                break;
            case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
                image_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                break;
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
                image_barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                break;
            case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
                image_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                break;
            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
                image_barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                break;
            default:break;
        }

        switch ( new_layout )
        {
            case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
                image_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                break;
            case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
                image_barrier.srcAccessMask |= VK_ACCESS_TRANSFER_READ_BIT;
                image_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                break;
            case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
                image_barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                image_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                break;
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
                image_barrier.dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                break;
            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
                image_barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
                image_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                break;
            default:break;
        }

        VkPipelineStageFlagBits src_flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        VkPipelineStageFlagBits dst_flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        vkCmdPipelineBarrier( command, src_flags, dst_flags, 0, 0, NULL, 0, NULL, 1, &image_barrier );
    }

    const vk::Texture* Texture::vulkan()const
    {
        return this;
    }

    const vk::Image& Texture::image()const
    {
        return _image;
    }
    vk::Image& Texture::image()
    {
        return _image;
    }
    uint32_t Texture::height()const
    {
        return _image.height();
    }

    uint32_t Texture::width()const
    {
        return _image.width();
    }

    uint32_t Texture::depth()const
    {
        return _image.depth();
    }

    void Texture::destruct()
    {
        _image.destruct();
    }

    Texture::~Texture()
    {
        destruct();
    }

    Texture::Texture()
    {}

}}
