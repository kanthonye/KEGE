//
//  decleared_types.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 2/2/23.
//

#ifndef KEGE_VULKAN_UTILS_HPP
#define KEGE_VULKAN_UTILS_HPP
#define GLFW_INCLUDE_VULKAN
#define _DEBUG
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <optional>
#include <filesystem>
#include "kege-string.hpp"
#include "../../../../engine/utils/Hashmap.hpp"
#include "../../device/debug/kege-logger.hpp"
#include "../../device/core/graphics-types.hpp"
#include "../../device/core/Device.hpp"
#include "../../device/core/UniformLayout.hpp"

namespace kege{namespace vk{

    class Instance;
    class ValidationLayer;
    class Device;
    class CommandPool;
    class CommandBuffer;
    class PipelineLayout;
    class DescriptorPool;
    class DescriptorSetLayout;
    class PipelineLayoutManager;

#ifdef _DEBUG
    static const bool KEGE_VULKAN_VALIDATION_LAYER_ENABLES = true;
#else
    static const bool KEGE_VULKAN_VALIDATION_LAYER_ENABLES = false;
#endif

    static const int MAX_FRAMES_IN_FLIGHT = 2;

    struct PhysicalDeviceRequirements
    {
        bool _graphics;
        bool _present;
        bool _compute;
        bool _transfer;
        bool _descrete_gpu;
        bool _sampler_anisotropy;
        std::vector< std::string > _device_extension_names;
    };

    struct DeviceQueueFamily
    {
        std::optional< uint32_t >& operator[]( core::CmdQueueType i )
        {
            return _indices[ i ];
        }

        bool isComplete()
        {
            return
            (
             _indices[ core::GRAPHICS_QUEUE ].has_value() &&
             _indices[ core::PRESENT_QUEUE  ].has_value()
            );
        }

        std::optional< uint32_t > _indices[ core::MAX_QUEUE_COUNT ];
    };

}}


namespace kege{namespace vk{

    class Semaphore
    {
    public:

        const VkSemaphore handle()const;
        VkResult construct();
        void destruct();
        ~Semaphore();

    private:

        VkSemaphore _handle;
    };

}}

namespace kege{namespace vk{

    class Fence
    {
    public:

        const VkFence handle()const;
        VkResult construct();
        void destruct();
        void reset();
        ~Fence();

    private:
        VkFence _handle;
    };
    
}}


namespace kege{namespace vk{

    void imageTransitionLayout( VkCommandBuffer command_buffer, VkImage image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout );

    int32_t getPhysicalDeviceMemoryTypeIndex( VkMemoryRequirements memory_requirements, VkMemoryPropertyFlags memory_properties );

    std::vector< const char*> getRequiredExtensions();

    bool physicalMeetDeviceRequirements
    (
        VkSurfaceKHR surface,
        VkPhysicalDevice physical_devices,
        VkPhysicalDeviceProperties* properties,
        VkPhysicalDeviceFeatures* features,
        VkPhysicalDeviceMemoryProperties* memory,
        PhysicalDeviceRequirements* requirements,
        DeviceQueueFamily& queue_family
    );

    bool checkDeviceExtensionSupport
    (
        VkPhysicalDevice physical_device,
        const std::vector< const char* >& device_extensions
    );

    bool isPhysicalDeviceSuitable
    (
        VkSurfaceKHR surface,
        VkPhysicalDevice physical_device,
        const std::vector< const char* >& device_extensions
    );

    void setDebugMessengerCreateInfo( VkDebugUtilsMessengerCreateInfoEXT& create_info );

    VkResult debugUtilsMessengerEXT
    (
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger
    );

    void getPhysicalDeviceSwapChainSupport
    (
        VkSurfaceKHR surface,
        VkPhysicalDevice physical_device,
        VkSurfaceCapabilitiesKHR& capabilities,
        std::vector<VkSurfaceFormatKHR>& formats,
        std::vector<VkPresentModeKHR>& present_modes
    );

    VkSurfaceFormatKHR swapSurfaceFormat
    (
        const std::vector< VkSurfaceFormatKHR >& available_formats
    );

    VkPresentModeKHR swapPresentMode
    (
        const std::vector< VkPresentModeKHR >& available_present_modes
    );

    VkExtent2D swapExtent
    (
        GLFWwindow* window,
        const VkSurfaceCapabilitiesKHR& capabilities
    );

    VkFormat supportedFormat
    (
         VkPhysicalDevice physical_device,
         const std::vector<VkFormat>& candidates,
         VkImageTiling tiling,
         VkFormatFeatureFlags features
    );


    VkSemaphore creatorSemaphore( VkDevice device );
    VkFence createFence( VkDevice device );
    void destroySemaphore( VkDevice device, VkSemaphore& semaphore );
    void destroyFence( VkDevice device, VkFence& fence );

    bool readfile( std::vector< char >& buffer, const std::string& filename );

    std::string getFileEXT( const std::string& filename );

    std::string creatSPIRVFileFromGLSLFile( const std::string& filename );

    bool convertGLSLToSPIRVFile( std::vector< char >& shader_bytecode, const std::string& filename );

    /**
     load SPIR-V file, or GLSL file. if the @b filename ext is not an  spv, then the file is converted to SPIR-V, loaded, and the content of the file is returned
     @param filename should be an SPIR-V file, or GLSL file.
     @return SPIR-V binary file data
     */
    bool loadShaderSource( std::vector< char >& source, const std::string& filename );

    VkVertexInputRate getVertexInputRate( core::VertexInputRate::Enum rate );
}}

#include "spirv.h"
#include "spirv_reflect.h"

namespace kege{namespace vk{

    void parseVertexInputAttributes
    (
         VkShaderStageFlagBits shader_type,
         SpvReflectShaderModule& shader_module,
         std::vector< VkVertexInputAttributeDescription >& vertex_attributes,
         std::vector< VkVertexInputBindingDescription >& vertex_bindings
    );

    void parsePushConstantRange
    (
        VkShaderStageFlagBits shader_type,
        SpvReflectShaderModule& shader_module,
        std::vector< VkPushConstantRange >& push_constants
    );

//    void parseDescriptorSetLayout
//    (
//        VkShaderStageFlagBits shader_type,
//        SpvReflectShaderModule& shader_module,
//        std::vector< kege::Ref< vk::DescriptorSetLayout > >& descriptor_set_layouts
//    );

    size_t vkFormatSize(VkFormat format);
    VkFormat toVkFormat( SpvReflectFormat format );
    VkDescriptorType toVkDescriptorType( SpvReflectDescriptorType descriptor_type );
    VkPolygonMode getPolygonMode( core::Polygon::Mode mode );
    VkPrimitiveTopology getPrimitiveTopology( core::Primitive::Type topology );

    VkFormat getVkFormat( core::ImgFormat format );
    VkFilter getFilter( core::Sampler::Filter filter );
    VkSamplerAddressMode getSamplerAddressMode( core::Sampler::Wrapping wrap );

}}

namespace kege{namespace vk{

    const char* vkGetError( VkResult result );
}}
#endif /* KEGE_VULKAN_UTILS_HPP */
