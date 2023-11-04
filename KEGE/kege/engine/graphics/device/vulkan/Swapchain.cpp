//
//  SwapChain.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/9/23.
//

#include "../../device/vulkan/Device.hpp"
#include "../../device/vulkan/Swapchain.hpp"
namespace kege{namespace vk{

    const vk::Framebuffer* Swapchain::frameBuffer( uint32_t index )const
    {
        return &_framebuffers[ index ];
    }

    const VkSwapchainKHR Swapchain::handle()const
    {
        return _swapchain;
    }

    VkResult Swapchain::construct()
    {
        vk::Device* device = vk::Device::get();
        
        // CREATE SWAPCHAIN -----------------------------

        VkSurfaceFormatKHR surface_format = swapSurfaceFormat( device->formats() );
        VkPresentModeKHR present_mode = swapPresentMode( device->presentModes() );
        VkExtent2D extent = swapExtent( device->window()->vulkan()->handle(), device->capabilities() );
        VkFormat format = surface_format.format;

        uint32_t image_count = MAX_FRAMES_IN_FLIGHT;//_device->capabilities().minImageCount;// + 1;
        if ( device->capabilities().maxImageCount > 0 && image_count > device->capabilities().maxImageCount )
        {
            image_count = device->capabilities().maxImageCount;
        }

        VkSwapchainCreateInfoKHR create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        create_info.surface = device->surface();
        create_info.minImageCount = image_count;
        create_info.imageFormat = surface_format.format;
        create_info.imageColorSpace = surface_format.colorSpace;
        create_info.imageExtent = extent;
        create_info.imageArrayLayers = 1;
        create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queue_family_indices[] =
        {
            device->getDeviceQueueFamily()[ core::GRAPHICS_QUEUE ].value(),
            device->getDeviceQueueFamily()[ core::PRESENT_QUEUE ].value()
        };

        if (device->getDeviceQueueFamily()[ core::GRAPHICS_QUEUE ].value() !=
            device->getDeviceQueueFamily()[ core::PRESENT_QUEUE ].value() )
        {
            create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            create_info.queueFamilyIndexCount = 2;
            create_info.pQueueFamilyIndices = queue_family_indices;
        }
        else
        {
            create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        create_info.preTransform = device->capabilities().currentTransform;
        create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        create_info.presentMode = present_mode;
        create_info.clipped = VK_TRUE;

        VkResult result = vkCreateSwapchainKHR( device->logicalDevice(), &create_info, nullptr, &_swapchain );
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR("%s : swapchain -> vkCreateSwapchainKHR()", vkGetError( result ));
            return result;
        }

        // get swapchain image count. get swapchain images
        std::vector< VkImage > images;
        vkGetSwapchainImagesKHR( device->logicalDevice(), _swapchain, &image_count, nullptr );
        images.resize( image_count );
        vkGetSwapchainImagesKHR( device->logicalDevice(), _swapchain, &image_count, images.data() );

        /*
         CREATE DEPTHBUFFER -----------------------------
         */

        VkFormat depth_format = supportedFormat
        (   device->physicalDevice()
         ,  {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}
         ,  VK_IMAGE_TILING_OPTIMAL
         ,  VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
         );

        VkImageCreateInfo image_info{};
        image_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
        image_info.samples       = VK_SAMPLE_COUNT_1_BIT;
        image_info.extent.width  = extent.width;
        image_info.extent.height = extent.height;
        image_info.extent.depth  = 1;
        image_info.mipLevels     = 1;
        image_info.arrayLayers   = 1;
        image_info.format        = depth_format;
        image_info.imageType     = VK_IMAGE_TYPE_2D;
        image_info.tiling        = VK_IMAGE_TILING_OPTIMAL;
        image_info.usage         = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        image_info.flags         = 0; // Optional

        vk::Texture* depth_texture = new vk::Texture;
        result = depth_texture->image().construct( image_info, VK_IMAGE_VIEW_TYPE_2D, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT );
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR("%s : swapchain -> depth_texture!", vkGetError( result ));
            return result;
        }

        /*
         CREATE RENDERPASS -----------------------------
         */

        kege::Ref< vk::Renderpass > renderpass = new vk::Renderpass;
        renderpass->addColorAttachment( format, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR );
        renderpass->addDepthAttachment( depth_format, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL );
        result = renderpass->construct();
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR("%s : swapchain -> renderpass!", vkGetError( result ));
            return result;
        }

        /*
         CREATE FRAMEBUFFER -----------------------------
         */

        // create swapchain framebuffers using swapchain images as render-targets
        _framebuffers.resize( image_count );
        for (size_t i = 0; i < image_count; i++)
        {
            VkExtent3D lenghts = { extent.width, extent.height, 1 };
            vk::Texture* color_texture = new vk::Texture;
            result = color_texture->image().construct( images[ i ], format, lenghts, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT );
            if ( result != VK_SUCCESS )
            {
                KEGE_ERROR("%s : swapchain -> framebuffer color_texture!", vkGetError( result ));
                destruct();
                return result;
            }
            _framebuffers[i].attach( core::COLOR_COMPONENT, color_texture );
            _framebuffers[i].attach( core::DEPTH_COMPONENT, depth_texture );
            _framebuffers[i].construct( renderpass );
        }

        return result;
    }
    
    void Swapchain::destruct()
    {
        _framebuffers.clear();
        if ( _swapchain )
        {
            vkDestroySwapchainKHR( vk::Device::get()->logicalDevice(), _swapchain, nullptr );
            _swapchain = VK_NULL_HANDLE;
        }
    }

    Swapchain::~Swapchain()
    {
        destruct();
    }
    Swapchain::Swapchain()
    :   _swapchain( VK_NULL_HANDLE )
    {}

}}






