//
//  ImageView.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/21/23.
//

#ifndef KEGE_VULKAN_IMAGE_VIEW_HPP
#define KEGE_VULKAN_IMAGE_VIEW_HPP

#include "../vulkan/utils.hpp"

namespace kege{namespace vk{

    class ImageView
    {
    public:
        
        VkResult construct( VkImage handle, VkFormat format, VkImageViewType view_type, VkImageAspectFlags aspect_flags );
        const VkImageView handle()const;
        void destruct();
        ~ImageView();
        ImageView();

        VkImageViewType _type;
        VkImageView _handle;
    };
}}

#endif /* KEGE_VULKAN_IMAGE_VIEW_HPP */
