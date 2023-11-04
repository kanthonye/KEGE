//
//  Device.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/11/23.
//

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../../device/vulkan/Device.hpp"
#include "../../device/vulkan/Descriptor.hpp"
#include "../../device/vulkan/RenderPass.hpp"
#include "../../device/vulkan/Window.hpp"
namespace kege{namespace vk{

    kege::Ref< core::Shader > Device::createGraphicsShader( const core::Shader::Info& info, const core::Renderpass* renderpass )
    {
        vk::GraphicsShader* shader = new vk::GraphicsShader();
        VkResult result = shader->construct( info, renderpass );
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR( "%s: Device::createGraphicsShader() failed.", vk::vkGetError( result ) );
            return nullptr;
        }
        return shader;
    }

    kege::Ref< core::Shader > Device::createComputeShader( const core::Shader::Info& info )
    {
        vk::ComputeShader* shader = new vk::ComputeShader();
        VkResult result = shader->construct( info );
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR( "%s: Device::createComputeShader().", vk::vkGetError( result ) );
            return nullptr;
        }
        return shader;
    }

    kege::Ref< core::CommandBuffer > Device::createCommandBuffer( core::CmdQueueType quetype )
    {
        return new vk::CommandBuffer( &_command_pools[ quetype ] );
    }
    
    kege::Ref< core::Framebuffer > Device::createFramebuffer()
    {
        return new vk::Framebuffer();
    };

    kege::Ref< core::Texture > Device::createTexture2d
    (
        uint32_t width,
        uint32_t height,
        uint32_t mip_levels,
        core::ImgFormat format,
        const void* data,
        bool storage
    )
    {
        vk::Texture* texture = new vk::Texture();
        texture->constructTexture2d
        (
            width,
            height,
            mip_levels,
            format,
            data,
            storage
        );
        return texture;
    }

    kege::Ref< core::Sampler > Device::createSampler
    (
        core::Sampler::Filter min,
        core::Sampler::Filter mag,
        core::Sampler::Wrapping x,
        core::Sampler::Wrapping y,
        core::Sampler::Wrapping z
    )
    {
        kege::Ref< vk::Sampler > sampler = new vk::Sampler;
        VkResult result = sampler->construct( min, mag, x, y, z );
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR( "%s: Device::createSampler() failed.", vk::vkGetError( result ) );
            return nullptr;
        }
        return sampler.ref();
    }

    kege::Ref< core::Buffer > Device::createBuffer
    (
        core::Buffer::Type type,
        uint64_t size,
        const void* data,
        core::Buffer::Usage usage
    )
    {
        kege::Ref< vk::Buffer > buffer = new vk::Buffer;
        VkResult result = buffer->construct
        (
            type, size, data, usage
        );
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR( "%s: Device::createBuffer() failed.", vk::vkGetError( result ) );
            throw std::runtime_error("\n");
            //return nullptr;
        }
        return buffer.ref();
    }

    void Device::destroyWindow( core::Window* window )
    {
        if( window != nullptr )
        {
            delete window;
            window = nullptr;
        }
    }

    const VkPhysicalDeviceProperties& Device::getPhysicalDeviceProperties()const
    {
        return _physical_device_properties;
    }
    
    vk::CommandPool* Device::commandPool( core::CmdQueueType quetyp )
    {
        return &_command_pools[ quetyp ];
    }

    const std::vector< VkPresentModeKHR >& Device::presentModes()const
    {
        return _present_modes;
    }
    const std::vector< VkSurfaceFormatKHR >& Device::formats()const
    {
        return _formats;
    }

    const VkPhysicalDeviceMemoryProperties& Device::physicalDeviceMemoryProperties()const
    {
        return _physical_device_memory_properties;
    }

    const VkFormatProperties& Device::getFormatProperties()const
    {
        return _format_properties;
    }

    const VkSurfaceCapabilitiesKHR& Device::capabilities()const
    {
        return _capabilities;
    }
    
    const VkPhysicalDevice Device::physicalDevice()const
    {
        return _physical_device;
    }

    const VkDevice Device::logicalDevice()const
    {
        return _device;
    }

    const VkSurfaceKHR& Device::surface()const
    {
        return _surface;
    }

    core::Renderer* Device::renderer()
    {
        return &_renderer;
    }

    core::Window* Device::window()
    {
        return _window;
    }
    
    VkQueue& Device::queues( uint32_t index )
    {
        return _device_queues[ index ];
    }

    vk::PipelineLayoutManager* Device::getPipelineLayoutManager()
    {
        return &_pipeline_layout_manager;
    }

    vk::DeviceQueueFamily& Device::getDeviceQueueFamily()
    {
        return _device_queue_family;
    }

    VkIndexType Device::getIndexType( uint32_t i )const
    {
        return _vk_index_types[i];
    }

    void Device::copyBufferToImage( const vk::Buffer& buffer, vk::Image& image )
    {
        // allocate a single command buffer from the command pool
        VkCommandBuffer cmdbuf = _command_pools[ core::TRANSFER_QUEUE ].allocateCommandBuffers( VK_COMMAND_BUFFER_LEVEL_PRIMARY );

        // begin recording commands into the command buffer
        VkCommandBufferBeginInfo begin_info{};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer( cmdbuf, &begin_info );

        imageTransitionLayout
        (
            cmdbuf,
            image.source().handle(),
            image.format(),
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        );

        // copy buffer data to image
        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = { image.width(), image.height(), 1 };
        vkCmdCopyBufferToImage( cmdbuf, buffer.handle(), image.source().handle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region );

        // To be able to start sampling from the texture image in the shader, we need to prepare it for shader access
        imageTransitionLayout
        (
            cmdbuf, image.source().handle(), image.format(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );

        vkEndCommandBuffer( cmdbuf );

        // submit the command buffer to the graphics queue for execution
        VkSubmitInfo submit_info = {};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &cmdbuf;
        vkQueueSubmit( _device_queues[ core::TRANSFER_QUEUE ], 1, &submit_info, VK_NULL_HANDLE );
        vkQueueWaitIdle( _device_queues[ core::TRANSFER_QUEUE ] );

        // free the command buffer and command pool
        _command_pools[ core::TRANSFER_QUEUE ].freeCommandBuffer( cmdbuf );
    }

    void Device::copyBuffer( vk::Buffer& dst, const vk::Buffer& src )
    {
        // allocate a single command buffer from the command pool
        VkCommandBuffer cmdbuf = _command_pools[ core::TRANSFER_QUEUE ].allocateCommandBuffers( VK_COMMAND_BUFFER_LEVEL_PRIMARY );

        // begin recording commands into the command buffer
        VkCommandBufferBeginInfo begin_info{};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer( cmdbuf, &begin_info );

        // record commands here
        VkBufferCopy copy_region{};
        copy_region.srcOffset = 0; // Optional
        copy_region.dstOffset = 0; // Optional
        copy_region.size = src.sizeInBytes();
        vkCmdCopyBuffer( cmdbuf, src.handle(), dst.handle(), 1, &copy_region );

        // end recording commands into the command buffer
        vkEndCommandBuffer( cmdbuf );

        // submit the command buffer to the graphics queue for execution
        VkSubmitInfo submit_info = {};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &cmdbuf;
        vkQueueSubmit( _device_queues[ core::TRANSFER_QUEUE ], 1, &submit_info, VK_NULL_HANDLE );
        vkQueueWaitIdle( _device_queues[ core::TRANSFER_QUEUE ] );

        // free the command buffer and command pool
        _command_pools[ core::TRANSFER_QUEUE ].freeCommandBuffer( cmdbuf );
    }

    VkCommandBuffer Device::allocateCommandBuffers( VkCommandPool pool )
    {
        VkCommandBufferAllocateInfo alloc_enfo{};
        alloc_enfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_enfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_enfo.commandPool = pool;
        alloc_enfo.commandBufferCount = 1;

        VkCommandBuffer buffer;
        if ( vkAllocateCommandBuffers( _device, &alloc_enfo, &buffer ) != VK_SUCCESS )
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }
        return buffer;
    }

    VkPhysicalDevice Device::selectPhysicalDevice( VkSurfaceKHR surface )
    {
        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices( _instance.handle(), &device_count, nullptr );

        if (device_count == 0)
        {
            KEGE_FATAL("failed to find GPUs with Vulkan support!");
            return VK_NULL_HANDLE;
        }

        VkPhysicalDevice physical_device = VK_NULL_HANDLE;
        VkPhysicalDevice physical_devices[ device_count ];
        vkEnumeratePhysicalDevices( _instance.handle(), &device_count, physical_devices );

        for (int i=0; i<device_count; i++)
        {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties( physical_devices[i], &properties );

            VkPhysicalDeviceFeatures features;
            vkGetPhysicalDeviceFeatures( physical_devices[i], &features );

            VkPhysicalDeviceMemoryProperties memory;
            vkGetPhysicalDeviceMemoryProperties( physical_devices[i], &memory );

            PhysicalDeviceRequirements requirements = {};
            requirements._graphics = true;
            requirements._present = true;
            requirements._compute = true;
            requirements._transfer = true;
            requirements._descrete_gpu = true;
            requirements._sampler_anisotropy = true;
            requirements._device_extension_names = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

            bool device_is_sutable = physicalMeetDeviceRequirements
            (
                surface,
                physical_devices[i],
                &properties,
                &features,
                &memory,
                &requirements,
                _device_queue_family
            );

            if ( device_is_sutable )
            {

                physical_device = physical_devices[i];

                KEGE_INFO
                (
                    "GPU Drive Version: %i.%i.%i",
                    VK_VERSION_MAJOR( properties.driverVersion),
                    VK_VERSION_MINOR( properties.driverVersion),
                    VK_VERSION_PATCH( properties.driverVersion)
                );

                KEGE_INFO
                (
                    "Vulkan API Version: %i.%i.%i",
                    VK_VERSION_MAJOR( properties.apiVersion),
                    VK_VERSION_MINOR( properties.apiVersion),
                    VK_VERSION_PATCH( properties.apiVersion)
                );

                for (int i=0; i<memory.memoryHeapCount; i++)
                {
                    double memsize = double( memory.memoryHeaps[i].size ) / ( 1024.0 * 1024.0 * 1024.0 );

                    if ( memory.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT )
                    {
                        KEGE_INFO( "Local GPU Memory: %.2f GiB", memsize );
                    }
                    else
                    {
                        KEGE_INFO( "Shared System Memory: %.2f GiB", memsize );
                    }
                }

                _physical_device_properties = properties;
                _physical_device_features = features;
                _physical_device_memory_properties = memory;
                break;
            }

            if ( _device_queue_family.isComplete() && isPhysicalDeviceSuitable( surface, physical_devices[i], _device_extensions ) )
            {
                _physical_device_properties = properties;
                _physical_device_features = features;
                _physical_device_memory_properties = memory;
                physical_device = physical_devices[i];
                break;
            }
        }

        if ( physical_device == VK_NULL_HANDLE )
        {
            KEGE_FATAL("failed to find a suitable GPU!");
            return VK_NULL_HANDLE;
        }
        return physical_device;
    }

    VkResult Device::createDevice()
    {
        _physical_device = selectPhysicalDevice( _surface );
        getPhysicalDeviceSwapChainSupport( _surface, _physical_device, _capabilities, _formats, _present_modes );

        std::set< uint32_t > unique_queue_families =
        {
            _device_queue_family[ core::GRAPHICS_QUEUE ].value(),
            _device_queue_family[ core::COMPUTE_QUEUE  ].value(),
            _device_queue_family[ core::PRESENT_QUEUE  ].value(),
            _device_queue_family[ core::TRANSFER_QUEUE ].value()
        };

        float queue_priority = 1.0f;
        std::vector< VkDeviceQueueCreateInfo > queue_create_infos;
        for ( uint32_t queue_family : unique_queue_families )
        {
            VkDeviceQueueCreateInfo info{};
            info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            info.queueFamilyIndex = queue_family;
            info.queueCount = 1;
            info.pQueuePriorities = &queue_priority;
            queue_create_infos.push_back(info);
        }

        VkPhysicalDeviceFeatures device_features{};
        device_features.fillModeNonSolid    = VK_TRUE; // Enable the fillModeNonSolid feature
        //device_features.geometryShader      = VK_TRUE; // Enable the geometryShader feature
        device_features.tessellationShader  = VK_TRUE; // Enable the tessellationShader feature
        device_features.imageCubeArray      = VK_TRUE;

        VkDeviceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.queueCreateInfoCount = static_cast<uint32_t>( queue_create_infos.size() );
        create_info.pQueueCreateInfos = queue_create_infos.data();
        create_info.pEnabledFeatures = &device_features;
        create_info.enabledExtensionCount = static_cast<uint32_t>( _device_extensions.size() );
        create_info.ppEnabledExtensionNames = _device_extensions.data();

        if ( KEGE_VULKAN_VALIDATION_LAYER_ENABLES )
        {
            create_info.enabledLayerCount = static_cast<uint32_t>( _layers.size());
            create_info.ppEnabledLayerNames = _layers.data();
        }
        else
        {
            create_info.enabledLayerCount = 0;
        }

        return vkCreateDevice( _physical_device, &create_info, nullptr, &_device );
    }


    void Device::createCommandPoolAndDeviceQueue( core::CmdQueueType queue )
    {
        vkGetDeviceQueue
        (
            _device,
            _device_queue_family[ queue ].value(),
            0, &_device_queues[ queue ]
        );

        _command_pools[ queue ].construct
        (
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, queue
        );
    }

    bool Device::construct( vk::Window* window )
    {
        if ( _self != this && _self != nullptr )
        {
            KEGE_FATAL( "multiple vk::Device instance is not supported" );
            return false;
        }
        _self = this;
        const VkDebugUtilsMessengerCreateInfoEXT *msg_create_info = nullptr;

        if ( KEGE_VULKAN_VALIDATION_LAYER_ENABLES )
        {
            _debug_messenger = new vk::DebugUtilsMessenger();
            msg_create_info = &_debug_messenger->info();
            _layers.push_back( _debug_messenger->sid().c_str() );
        }

        VkResult result = _instance.construct( msg_create_info, &_layers );
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR( vkGetError( result ) );
            return false;
        }

        if ( _debug_messenger != nullptr )
        {
            result = _debug_messenger->construct( _instance );
            if ( result != VK_SUCCESS )
            {
                destruct();
                KEGE_ERROR( vkGetError( result ) );
                return false;
            }
        }

        _window = window;

        result = glfwCreateWindowSurface( _instance.handle(), window->handle(), nullptr, &_surface );
        if ( result != VK_SUCCESS )
        {
            destruct();
            KEGE_ERROR( vkGetError( result ) );
            return false;
        }

        result = createDevice();
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR( vkGetError( result ) );
            destruct();
            return false;
        }

        _command_pools.resize( core::MAX_QUEUE_COUNT );
        createCommandPoolAndDeviceQueue( core::GRAPHICS_QUEUE );
        createCommandPoolAndDeviceQueue( core::COMPUTE_QUEUE  );
        createCommandPoolAndDeviceQueue( core::TRANSFER_QUEUE );
        createCommandPoolAndDeviceQueue( core::PRESENT_QUEUE  );

        //vkGetPhysicalDeviceMemoryProperties( _physical_device, &_physical_device_memory_properties );
        //vkGetPhysicalDeviceProperties( _physical_device, &_physical_device_properties);
        _min_ubo_offset_alignment = (uint32_t) _physical_device_properties.limits.minUniformBufferOffsetAlignment;

        std::cout <<"max-storage-buffer-size: "<< _physical_device_properties.limits.maxStorageBufferRange <<"\n";

        result = _renderer.construct();
        if ( result != VK_SUCCESS )
        {
            destruct();
            return false;
        }
        return true;
    }

    void Device::destruct()
    {
        _renderer.destruct();
        _pipeline_layout_manager.destruct();
        
        for ( auto& pool : _command_pools ) pool.destruct();
        _command_pools.clear();

        if ( _device != nullptr )
        {
            vkDestroyDevice( _device, nullptr );
            _device = VK_NULL_HANDLE;
        }

        if ( _surface != nullptr )
        {
            vkDestroySurfaceKHR( _instance.handle(), _surface, nullptr );
            _surface = VK_NULL_HANDLE;
        }

        _debug_messenger.clear();
        _instance.destruct();

        if( _window != nullptr )
        {
            delete _window;
            _window = nullptr;
        }
        glfwTerminate();
    }

    Device* Device::get()
    {
        return _self;
    }
    
    void Device::waitIdle()
    {
        vkDeviceWaitIdle( _device );
    }

    Device::~Device()
    {
        destruct();
    }

    Device::Device()
    :   _window( nullptr )
    ,   _debug_messenger( nullptr )
    ,   _device( VK_NULL_HANDLE )
    ,   _surface( VK_NULL_HANDLE )
    ,   _physical_device( VK_NULL_HANDLE )
    ,   _device_queues{ VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE }
    {
        if ( !glfwInit() ) // init glfw
        {
            KEGE_FATAL(" failed to initialize GLFW!" );
            _self = this;
        }

        /* GLFW was originally designed to create an OpenGL context, we need
         to tell it to not create an OpenGL context with GLFW_NO_API */
        glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );


        _vk_index_types[ core::Index::UI8  ] = VK_INDEX_TYPE_UINT8_EXT;
        _vk_index_types[ core::Index::UI16 ] = VK_INDEX_TYPE_UINT16;
        _vk_index_types[ core::Index::UI32 ] = VK_INDEX_TYPE_UINT32;
    }

    vk::Device* Device::_self = nullptr;
}}
