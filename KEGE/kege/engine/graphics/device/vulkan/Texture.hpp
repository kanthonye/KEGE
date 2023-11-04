//
//  Texture.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/21/23.
//

#ifndef KEGE_VULKAN_TEXTURE_HPP
#define KEGE_VULKAN_TEXTURE_HPP

#include "../vulkan/Image.hpp"
#include "../vulkan/Sampler.hpp"

namespace kege{namespace vk{
    
    class Texture : public kege::core::Texture
    {
    public:

        VkResult constructTexture2d
        (
            uint32_t width,
            uint32_t height,
            uint32_t mip_levels,
            core::ImgFormat format,
            const void* data,
            bool storage
        );

        VkResult constructTexture3d
        (
            uint32_t width,
            uint32_t height,
            uint32_t depth,
            uint32_t mip_levels,
            core::ImgFormat format,
            const void* data
        );

        VkResult constructTextureArray
        (
            uint32_t width,
            uint32_t height,
            uint32_t layers,
            uint32_t mip_levels,
            core::ImgFormat format,
            const void* data
        );

        VkResult operator()
        (
            const VkImageCreateInfo& create_info,
            VkImageViewType view_type,
            VkMemoryPropertyFlags memory_property,
            VkImageAspectFlags aspect_mask,
            const void* data
        );

        void setImageLayout( VkCommandBuffer command, VkImageLayout new_layout );

        const vk::Texture* vulkan()const;

        const vk::Image& image()const;
        vk::Image& image();

        uint32_t height()const;
        uint32_t width()const;
        uint32_t depth()const;
        void destruct();
        
        ~Texture();
        Texture();

    protected:

        vk::Image _image;
    };

}}

#endif /* KEGE_VULKAN_TEXTURE_HPP */
