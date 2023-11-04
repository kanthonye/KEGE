//
//  Image.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/11/23.
//

#ifndef KEGE_VULKAN_IMAGE
#define KEGE_VULKAN_IMAGE

#include "../vulkan/ImageSrce.hpp"
#include "../vulkan/ImageView.hpp"

namespace kege{namespace vk{

    class Image : public kege::RefCounter
    {
    public:

        VkResult construct
        (
            const VkImageCreateInfo& info,
            VkImageViewType view_type,
            VkMemoryPropertyFlags property_bitflags,
            VkImageAspectFlags aspect_flags
        );

        /* this method is design for the vulkan swapchain framebuffer initialization. */
        VkResult construct
        (
            VkImage image,
            VkFormat format,
            VkExtent3D extend,
            VkImageViewType view_type,
            VkImageAspectFlags aspect_flags
        );

        const vk::Image* vulkan()const;
        vk::Image* vulkan();
        const ImageSrce& source()const;
        const ImageView& view()const;
        VkFormat format()const;
        uint32_t height()const;
        uint32_t width()const;
        uint32_t depth()const;
        void destruct();

        ~Image();
        Image();

        VkImageAspectFlags _aspect_flags;
        VkImageLayout _layout;
        VkExtent3D _extend;
        VkFormat _format;
        ImageSrce _source;
        ImageView _view;
    };

}}
#endif /* Image_hpp */
