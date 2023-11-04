//
//  ImageView.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/21/23.
//

#include "../vulkan/Device.hpp"
#include "../vulkan/ImageView.hpp"

namespace kege{namespace vk{
//
//    bool checkFormat()
//    {
//        VkFormatProperties format_properties;
//        vkGetPhysicalDeviceFormatProperties( physical_device, format, &format_properties );
//
//        if( !(vk::Device::get()->_physical_device_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) )
//        {
//          std::cout << "Provided format is not supported for a sampled image." << std::endl;
//          return false;
//        }
//        if( linear_filtering && !(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT) )
//        {
//          std::cout << "Provided format is not supported for a linear image filtering." << std::endl;
//          return false;
//        }
//        return true;
//    }

    VkResult ImageView::construct( VkImage image, VkFormat format, VkImageViewType type, VkImageAspectFlags aspect_flags )
    {
        VkDevice device = vk::Device::get()->logicalDevice();
        VkImageViewCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        info.image = image;
        info.format = format;
        info.viewType = type;
        info.subresourceRange.aspectMask = aspect_flags;
        info.subresourceRange.baseMipLevel = 0;
        info.subresourceRange.levelCount = 1;
        info.subresourceRange.baseArrayLayer = 0;
        info.subresourceRange.layerCount = 1;
        info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        
        return vkCreateImageView( device, &info, nullptr, &_handle );
    }

    const VkImageView ImageView::handle()const
    {
        return _handle;
    }

    void ImageView::destruct()
    {
        if ( _handle != VK_NULL_HANDLE )
        {
            vkDestroyImageView( vk::Device::get()->logicalDevice(), _handle, nullptr );
            _handle = VK_NULL_HANDLE;
        }
    }

    ImageView::~ImageView()
    {
        if ( _handle != VK_NULL_HANDLE )
        {
            destruct();
        }
    }
    
    ImageView::ImageView()
    :   _handle( VK_NULL_HANDLE )
    ,   _type( VK_IMAGE_VIEW_TYPE_1D )
    {}
}}
