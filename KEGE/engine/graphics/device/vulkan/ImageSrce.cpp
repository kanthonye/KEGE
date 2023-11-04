//
//  ImageSrce.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/21/23.
//

#include "../vulkan/Device.hpp"
#include "../vulkan/ImageSrce.hpp"
namespace kege{namespace vk{

    bool checkFormatCompatability( VkFormat format, VkFormatFeatureFlagBits format_feature )
    {
        VkFormatProperties format_properties;
        vkGetPhysicalDeviceFormatProperties( vk::Device::get()->physicalDevice(), format, &format_properties );
        if( !(format_properties.optimalTilingFeatures & format_feature) )
        {
          std::cout << "Provided format is not supported for a storage image." << std::endl;
          return false;
        }
        /*
        if( atomic_operations && !(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT) ) {
          std::cout << "Provided format is not supported for atomic operations on storage images." << std::endl;
          return false;
        }
         */
        return true;
    }



    VkResult createImage
    (
        VkDevice                logical_device,
        VkImageType             type,
        VkFormat                format,
        VkExtent3D              size,
        uint32_t                num_mipmaps,
        uint32_t                num_layers,
        VkSampleCountFlagBits   samples,
        VkImageUsageFlags       usage_scenarios,
        VkImageTiling           image_tiling,
        VkImageCreateFlags      image_create_flags,
        VkImage                 &image
    )
    {
        VkImageCreateInfo image_create_info =
        {
            VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,                // VkStructureType          sType
            nullptr,                                            // const void             * pNext
            image_create_flags,                                 // VkImageCreateFlags       flags   // cubemap ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0u, 
            type,                                               // VkImageType              imageType
            format,                                             // VkFormat                 format
            size,                                               // VkExtent3D               extent
            num_mipmaps,                                        // uint32_t                 mipLevels
            num_layers,                                         // uint32_t                 arrayLayers
            samples,                                            // VkSampleCountFlagBits    samples
            image_tiling,                                       // VkImageTiling            tiling
            usage_scenarios,                                    // VkImageUsageFlags        usage
            VK_SHARING_MODE_EXCLUSIVE,                          // VkSharingMode            sharingMode
            0,                                                  // uint32_t                 queueFamilyIndexCount
            nullptr,                                            // const uint32_t         * pQueueFamilyIndices
            VK_IMAGE_LAYOUT_UNDEFINED                           // VkImageLayout            initialLayout
        };

        VkResult result = vkCreateImage( logical_device, &image_create_info, nullptr, &image );
        if( result != VK_SUCCESS )
        {
            KEGE_ERROR( "vk::createImage() : Could not create an image : error -> %s.", vkGetError( result ) );
            return VK_ERROR_INITIALIZATION_FAILED;
        }
        return result;
    }

    VkResult createImageView
    (
        VkDevice            logical_device,
        VkImage             image,
        VkImageViewType     view_type,
        VkFormat            format,
        VkImageAspectFlags  aspect,
        VkImageView         &image_view
    ) 
    {
        VkImageViewCreateInfo image_view_create_info =
        {
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,       // VkStructureType            sType
            nullptr,                                        // const void               * pNext
            0,                                              // VkImageViewCreateFlags     flags
            image,                                          // VkImage                    image
            view_type,                                      // VkImageViewType            viewType
            format,                                         // VkFormat                   format

            {// VkComponentMapping
                VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle         r
                VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle         g
                VK_COMPONENT_SWIZZLE_IDENTITY,              // VkComponentSwizzle         b
                VK_COMPONENT_SWIZZLE_IDENTITY               // VkComponentSwizzle         a
            },
            {// VkImageSubresourceRange
                aspect,                                     // VkImageAspectFlags         aspectMask
                0,                                          // uint32_t                   baseMipLevel
                VK_REMAINING_MIP_LEVELS,                    // uint32_t                   levelCount
                0,                                          // uint32_t                   baseArrayLayer
                VK_REMAINING_ARRAY_LAYERS                   // uint32_t                   layerCount
            }
        };

        VkResult result = vkCreateImageView( logical_device, &image_view_create_info, nullptr, &image_view );
        if( result != VK_SUCCESS )
        {
            KEGE_ERROR( "vk::createImageView() : Could not create an image : error -> %s.", vkGetError( result ) );
            return VK_ERROR_INITIALIZATION_FAILED;
        }
        return result;
    }






    
    VkResult ImageSrce::construct( const VkImageCreateInfo& info, VkMemoryPropertyFlags memory_properties )
    {
        VkDevice device = vk::Device::get()->logicalDevice();
        VkResult result = vkCreateImage( device, &info, nullptr, &_handle );
        if ( result != VK_SUCCESS )
        {
            return result;
        }

        VkMemoryRequirements memory_requirements;
        vkGetImageMemoryRequirements( device, _handle, &memory_requirements );
        uint32_t memory_type_index = getPhysicalDeviceMemoryTypeIndex
        (
            memory_requirements, memory_properties
        );

        if( memory_type_index < 0 )
        {
            KEGE_ERROR( "Could not allocate memory for an image." );
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        VkMemoryAllocateInfo image_memory_allocate_info =
        {
            VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            nullptr,
            memory_requirements.size,
            memory_type_index
        };

        result = vkAllocateMemory( device, &image_memory_allocate_info, nullptr, &_memory );
        if( result != VK_SUCCESS )
        {
            KEGE_ERROR( "Could not allocate memory for an image." );
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        result = vkBindImageMemory( device, _handle, _memory, 0 );
        if( result != VK_SUCCESS )
        {
            KEGE_ERROR( "Could not bind memory object to an image." );
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        return result;
    }

    ImageSrce::ImageSrce(VkDeviceMemory memory, VkImage handle)
    {}

    const VkDeviceMemory ImageSrce::memory()const
    {
        return _memory;
    }

    const VkImage ImageSrce::handle()const
    {
        return _handle;
    }

    ImageSrce::ImageSrce(VkImage handle)
    :   _memory( VK_NULL_HANDLE )
    ,   _handle( handle )
    {}

    void ImageSrce::destruct()
    {
        if ( _memory != VK_NULL_HANDLE )
        {
            vkFreeMemory( vk::Device::get()->logicalDevice(), _memory, nullptr );
            vkDestroyImage( vk::Device::get()->logicalDevice(), _handle, nullptr );
            _memory = VK_NULL_HANDLE;
        }
        _handle = VK_NULL_HANDLE;
    }

    ImageSrce::~ImageSrce()
    {
        if ( _memory != VK_NULL_HANDLE )
        {
            destruct();
        }
    }

    ImageSrce::ImageSrce()
    :   _memory( VK_NULL_HANDLE )
    ,   _handle( VK_NULL_HANDLE )
    {}
}}
