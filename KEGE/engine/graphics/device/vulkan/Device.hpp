//
//  Device.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/11/23.
//

#ifndef KEGE_VULKAN_DEVICE
#define KEGE_VULKAN_DEVICE

#include "../../device/core/Device.hpp"
#include "../vulkan/utils.hpp"
#include "../vulkan/Image.hpp"
#include "../vulkan/Buffer.hpp"
#include "../vulkan/GraphicsShader.hpp"
#include "../vulkan/UniformHandle.hpp"
#include "../vulkan/ComputeShader.hpp"
#include "../vulkan/CommandBuffer.hpp"
#include "../vulkan/Window.hpp"
#include "../vulkan/Texture.hpp"
#include "../vulkan/Sampler.hpp"
#include "../vulkan/Renderer.hpp"

#include "../vulkan/Instance.hpp"
#include "../vulkan/DebugUtilsMessenger.hpp"
#include "../vulkan/CommandPool.hpp"

namespace kege{namespace vk{
    class PipelineLayout;
    class RenderPass;

    class Device : public kege::core::Device
    {
    public:

        kege::Ref< core::Shader > createGraphicsShader( const core::Shader::Info& info, const core::Renderpass* renderpass );
        kege::Ref< core::Shader > createComputeShader( const core::Shader::Info& info );
        kege::Ref< core::CommandBuffer > createCommandBuffer( core::CmdQueueType quetype );
        kege::Ref< core::Framebuffer > createFramebuffer();

        kege::Ref< core::Texture > createTexture2d
        (
            uint32_t width,
            uint32_t height,
            uint32_t mip_levels,
            core::ImgFormat format,
            const void* data,
            bool storage
        );

        kege::Ref< core::Sampler > createSampler
        (
            core::Sampler::Filter min,
            core::Sampler::Filter mag,
            core::Sampler::Wrapping x,
            core::Sampler::Wrapping y,
            core::Sampler::Wrapping z
        );

        kege::Ref< core::Buffer > createBuffer
        (
            core::Buffer::Type type,
            uint64_t size,
            const void* data,
            core::Buffer::Usage usage
        );

        void copyBufferToImage( const vk::Buffer& buffer, vk::Image& image );
        void copyBuffer( vk::Buffer& dst, const vk::Buffer& src );

        void destroyWindow( core::Window* window );;
        void freeSampler2d( const core::Sampler* value );

        const VkPhysicalDeviceProperties& getPhysicalDeviceProperties()const;
        vk::CommandPool* commandPool( core::CmdQueueType quetyp );
        const std::vector< VkPresentModeKHR >& presentModes()const;
        const std::vector< VkSurfaceFormatKHR >& formats()const;

        const VkPhysicalDeviceMemoryProperties& physicalDeviceMemoryProperties()const;
        const VkFormatProperties& getFormatProperties()const;
        const VkSurfaceCapabilitiesKHR& capabilities()const;
        const VkPhysicalDevice physicalDevice()const;
        const VkDevice logicalDevice()const;
        const VkSurfaceKHR& surface()const;
        CommandBuffer* createGraphicsCommandBuffer();
        CommandBuffer* createComputeCommandBuffer();

        core::Renderer* renderer();
        core::Window* window();

        VkQueue& queues( uint32_t index );
        //vk::DescriptorSetAllocators* descriptorSetAllocators();

        VkPipelineLayout createPipelineLayout
        (
            std::vector< VkDescriptorSetLayout >& descriptor_set_layouts,
            const std::vector< VkPushConstantRange >& push_constant_ranges
        );
        vk::PipelineLayoutManager* getPipelineLayoutManager();
        vk::DeviceQueueFamily& getDeviceQueueFamily();
        VkIndexType getIndexType( uint32_t i )const;

        bool construct( vk::Window* window );
        static Device* get();
        void destruct();
        void waitIdle();


        ~Device();
        Device();

    private:

        void createCommandPoolAndDeviceQueue( core::CmdQueueType queue );
        vk::Device& operator=( const vk::Device& device ) = delete;
        Device( const vk::Device& device ) = delete;

        VkCommandBuffer allocateCommandBuffers( VkCommandPool pool );
        VkPhysicalDevice selectPhysicalDevice( VkSurfaceKHR surface );
        VkResult createDevice();

    private:

        vk::PipelineLayoutManager _pipeline_layout_manager;

        VkPhysicalDeviceMemoryProperties _physical_device_memory_properties;
        VkPhysicalDeviceProperties _physical_device_properties;
        VkPhysicalDeviceFeatures _physical_device_features;
        VkFormatProperties _format_properties;

        std::vector< VkPresentModeKHR > _present_modes;
        std::vector< VkSurfaceFormatKHR > _formats;
        VkSurfaceCapabilitiesKHR _capabilities;

        uint32_t _min_ubo_offset_alignment;

        VkPhysicalDevice _physical_device;
        VkSurfaceKHR _surface;
        VkDevice _device;

        vk::DeviceQueueFamily _device_queue_family;
        std::vector< vk::CommandPool > _command_pools;
        VkQueue _device_queues[ core::MAX_QUEUE_COUNT ];

        // Set the required device extentions we need
        std::vector< const char* > _device_extensions =
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,

            /*
            I have been getting validation error everytime i ran vulkan project.

            Validation Error: [ VUID-VkDeviceCreateInfo-pProperties-04451 ]
            vkCreateDevice: VK_KHR_portability_subset must be enabled because
            physical device VkPhysicalDevice 0x600000245880[] supports it The
            Vulkan spec states: If the VK_KHR_portability_subset extension is
            included in pProperties of vkEnumerateDeviceExtensionProperties,
            ppEnabledExtensionNames must include "VK_KHR_portability_subset".

            To fix I added VK_KHR_get_physical_device_properties2 to instance
            extension and added VK_KHR_portability_subset to device extension */
            "VK_KHR_portability_subset",
        };

        std::vector<const char *> _layers;
        kege::Ref< vk::DebugUtilsMessenger > _debug_messenger;
        vk::Instance _instance;
        vk::Renderer _renderer;
        vk::Window* _window;

        /**
         * Vulkan index types for different index formats.
         */
        VkIndexType _vk_index_types[5];

        static vk::Device* _self;
    };
    
}}
#endif /* KEGE_VULKAN_DEVICE */
