//
//  Image.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/11/23.
//

#include "../vulkan/utils.hpp"
#include "../vulkan/Image.hpp"
#include "../vulkan/Buffer.hpp"
#include "../vulkan/Device.hpp"
namespace kege{namespace vk{

    VkResult Image::construct
    (
        const VkImageCreateInfo& info,
        VkImageViewType view_type,
        VkMemoryPropertyFlags property_bitflags,
        VkImageAspectFlags aspect_flags
    )
    {
        VkResult result = _source.construct( info, property_bitflags );
        if ( result != VK_SUCCESS )
        {
            KEGE_FATAL("%s : failed to create vk::ImageSrce!", vkGetError( result ));
            return result;
        }
        
        result = _view.construct( _source.handle(), info.format, view_type, aspect_flags );
        if ( result != VK_SUCCESS )
        {
            KEGE_FATAL( "%s : failed to create vk::ImageView.", vkGetError( result ) );
            return result;
        }
        _aspect_flags = aspect_flags;
        _layout = info.initialLayout;
        _format = info.format;
        _extend = info.extent;
        return result;
    }

    VkResult Image::construct( VkImage image, VkFormat format, VkExtent3D extend, VkImageViewType view_type, VkImageAspectFlags aspect_flags )
    {
        _source = vk::ImageSrce( image );
        _format = format;
        _extend = extend;
        return _view.construct( image, format, view_type, aspect_flags );
    }
    const vk::Image* Image::vulkan()const
    {
        return this;
    }
    
    vk::Image* Image::vulkan()
    {
        return this;
    }

    const ImageSrce& Image::source()const
    {
        return _source;
    }

    const ImageView& Image::view()const
    {
        return _view;
    }

    VkFormat Image::format()const
    {
        return _format;
    }

    uint32_t Image::height()const
    {
        return _extend.height;
    }

    uint32_t Image::width()const
    {
        return _extend.width;
    }

    uint32_t Image::depth()const
    {
        return _extend.depth;
    }
    
    void Image::destruct()
    {
        _source.destruct();
        _view.destruct();
    }

    Image::~Image()
    {
        destruct();
    }

    Image::Image()
    {}



//    VkResult createImage
//    (
//     VkImageType            image_type,
//     VkFormat               format,
//     VkExtent3D             extent,
//     uint32_t               mip_levels,
//     uint32_t               array_layers,
//     VkSampleCountFlagBits  samples,
//     VkImageTiling          tiling,
//     VkImageUsageFlags      usage
//    )
//    {
//        return {};
//    }

//    if( !CreateImage( logical_device, type, format, size, num_mipmaps, num_layers, VK_SAMPLE_COUNT_1_BIT, usage | VK_IMAGE_USAGE_STORAGE_BIT, false, storage_image ) ) {
//      return false;
//    }

//    if( !AllocateAndBindMemoryObjectToImage( physical_device, logical_device, storage_image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memory_object ) ) {
//      return false;
//    }
//
//    if( !CreateImageView( logical_device, storage_image, view_type, format, aspect, storage_image_view ) ) {
//      return false;
//    }
//    return true;
}}
