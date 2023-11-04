//
//  utils.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/10/23.
//

#include "utils.hpp"
#include "../vulkan/Buffer.hpp"
#include "../vulkan/Texture.hpp"
#include "../vulkan/Device.hpp"
#include "../vulkan/Descriptor.hpp"
namespace kege{namespace vk{

    const VkSemaphore Semaphore::handle()const
    {
        return _handle;
    }
    VkResult Semaphore::construct()
    {
        VkSemaphoreCreateInfo semaphore_info{};
        semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        VkResult result = vkCreateSemaphore( vk::Device::get()->logicalDevice(), &semaphore_info, nullptr, &_handle );
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR("%s: failed to create vk::Semaphore!", vk::vkGetError( result ) );
            return result;
        }
        return result;
    }

    void Semaphore::destruct()
    {
        if ( _handle != VK_NULL_HANDLE )
        {
            vk::Device::get()->waitIdle();
            destroySemaphore( vk::Device::get()->logicalDevice(), _handle );
            _handle = VK_NULL_HANDLE;
        }
    }
    Semaphore::~Semaphore()
    {
        destruct();
    }
}}

namespace kege{namespace vk{

    const VkFence Fence::handle()const
    {
        return _handle;
    }

    VkResult Fence::construct()
    {
        VkFenceCreateInfo fence_info{};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        VkResult result = vkCreateFence( vk::Device::get()->logicalDevice(), &fence_info, nullptr, &_handle );
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR("%s: failed to create vk::Fence!", vk::vkGetError( result ) );
            return result;
        }
        reset();
        return result;
    }

    void Fence::destruct()
    {
        if ( _handle != VK_NULL_HANDLE )
        {
            vk::Device::get()->waitIdle();
            destroyFence( vk::Device::get()->logicalDevice(), _handle );
            _handle = VK_NULL_HANDLE;
        }
    }
    void Fence::reset()
    {
        vkResetFences( vk::Device::get()->logicalDevice(), 1, &_handle );
    }

    Fence::~Fence()
    {
        destruct();
    }

}}



namespace kege{namespace vk{

    void imageTransitionLayout( VkCommandBuffer command_buffer, VkImage image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout )
    {
        VkImageMemoryBarrier barrier{};
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.oldLayout = old_layout;
        barrier.newLayout = new_layout;
        barrier.image = image;

        VkPipelineStageFlags source_stage;
        VkPipelineStageFlags destination_stage;

        if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        else
        {
            throw std::invalid_argument("unsupported layout transition!");
        }

        vkCmdPipelineBarrier
        (
             command_buffer,
             source_stage,
             destination_stage,
             0,
             0, nullptr,
             0, nullptr,
             1, &barrier
        );
    }

    int32_t getPhysicalDeviceMemoryTypeIndex( VkMemoryRequirements memory_requirements, VkMemoryPropertyFlags memory_properties )
    {
        const VkPhysicalDeviceMemoryProperties& physical_device_memory_properties = vk::Device::get()->physicalDeviceMemoryProperties();
        for( uint32_t type = 0; type < physical_device_memory_properties.memoryTypeCount; ++type )
        {
            if( (memory_requirements.memoryTypeBits & (1 << type)) && ((physical_device_memory_properties.memoryTypes[ type ].propertyFlags & memory_properties) == memory_properties) )
            {
                return type;
            }
        }
        return -1;
    }

    std::vector< const char*> getRequiredExtensions()
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions( &glfwExtensionCount );

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if ( KEGE_VULKAN_VALIDATION_LAYER_ENABLES )
        {
            extensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );

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
            extensions.push_back("VK_KHR_get_physical_device_properties2");
        }

        return extensions;
    }


    bool physicalMeetDeviceRequirements
    (
        VkSurfaceKHR surface,
        VkPhysicalDevice physical_devices,
        VkPhysicalDeviceProperties* properties,
        VkPhysicalDeviceFeatures* features,
        VkPhysicalDeviceMemoryProperties* memory,
        PhysicalDeviceRequirements* requirements,
        DeviceQueueFamily& queue_family
    )
    {
        if ( !requirements->_descrete_gpu )
        {
            if( properties->deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ) {
                KEGE_INFO( "device is not a descrete GPU, a descrete GPU is required, so this one is skipped." );
                return false;
            }
        }

        uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties( physical_devices, &queue_family_count, nullptr );
        VkQueueFamilyProperties queue_family_properties[ queue_family_count ];
        vkGetPhysicalDeviceQueueFamilyProperties( physical_devices, &queue_family_count, queue_family_properties );

        KEGE_INFO( "Graphics | Present | Compute | Transfer | Name" );

        for(int i=0; i<queue_family_count; i++)
        {
            int index_taken = 0;
            if( !queue_family[ core::GRAPHICS_QUEUE ].has_value() && queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT )
            {
                queue_family[ core::GRAPHICS_QUEUE ] = i;
                index_taken = 'G';
            }
            if( !queue_family[ core::COMPUTE_QUEUE ].has_value() && queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT )
            {
                if ( i >= 1 )
                {
                    queue_family[ core::COMPUTE_QUEUE ] = i;
                    index_taken = 'C';
                }
            }
            if( !queue_family[ core::TRANSFER_QUEUE ].has_value() && queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT )
            {
                if ( index_taken == 0 )
                {
                    queue_family[ core::TRANSFER_QUEUE ] = i;
                    index_taken = 'T';
                }
            }

            VkBool32 support_present = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR( physical_devices, i, surface, &support_present );
            if ( !queue_family[ core::PRESENT_QUEUE ].has_value() && support_present == VK_TRUE )
            {
                if ( index_taken == 'C' || index_taken == 0 )
                {
                    queue_family[ core::PRESENT_QUEUE ] = i;
                    index_taken = 'P';
                }
            }
        }

        KEGE_INFO( " %7d | %7d | %7d | %8d | %s",
            queue_family[ core::GRAPHICS_QUEUE ],
            queue_family[ core::COMPUTE_QUEUE  ],
            queue_family[ core::PRESENT_QUEUE  ],
            queue_family[ core::TRANSFER_QUEUE ],
            properties->deviceName
        );

        if
        (
            (!requirements->_graphics || (requirements->_graphics && queue_family[ core::GRAPHICS_QUEUE ].has_value())) &&
            (!requirements->_compute  || (requirements->_compute  && queue_family[ core::COMPUTE_QUEUE ].has_value()) ) &&
            (!requirements->_transfer || (requirements->_transfer && queue_family[ core::PRESENT_QUEUE ].has_value()) ) &&
            (!requirements->_present  || (requirements->_present  && queue_family[ core::TRANSFER_QUEUE ].has_value()) )
        )
        {
            KEGE_INFO( "Device %s meets queue requirements", properties->deviceName );
            KEGE_TRACE( "Graphics Queue family index %i", queue_family[ core::GRAPHICS_QUEUE ] );
            KEGE_TRACE( "Present  Queue family index %i", queue_family[ core::PRESENT_QUEUE  ] );
            KEGE_TRACE( "Transfer Queue family index %i", queue_family[ core::TRANSFER_QUEUE ] );
            KEGE_TRACE( "Compute  Queue family index %i", queue_family[ core::COMPUTE_QUEUE  ] );

            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> present_modes;
            getPhysicalDeviceSwapChainSupport( surface, physical_devices, capabilities, formats, present_modes );

            if ( formats.empty() || present_modes.empty() )
            {
                KEGE_INFO( "Required swapchain support not available for the device %s. Skipping device.", properties->deviceName );
                return false;
            }

            if ( requirements->_device_extension_names.size() )
            {
                uint32_t available_extenstion_count = 0;
                vkEnumerateDeviceExtensionProperties( physical_devices, nullptr, &available_extenstion_count, nullptr );
                if ( available_extenstion_count )
                {
                    std::vector< VkExtensionProperties > extension_properties;
                    extension_properties.resize( available_extenstion_count );
                    vkEnumerateDeviceExtensionProperties( physical_devices, nullptr, &available_extenstion_count, extension_properties.data() );

                    for (int i=0; i<requirements->_device_extension_names.size(); i++)
                    {
                        bool found = false;
                        for (int j=0; j<extension_properties.size(); j++)
                        {
                            if ( requirements->_device_extension_names[i] == extension_properties[j].extensionName )
                            {
                                found = true;
                                break;
                            }
                        }

                        if ( !found )
                        {
                            KEGE_INFO( "Required extension not available for the device %s, skipping device.", properties->deviceName );
                            return false;
                        }
                    }
                }
            }

            if ( requirements->_sampler_anisotropy && !features->samplerAnisotropy )
            {
                KEGE_INFO( "Device does not support not sampler-anisotropy, skipping device.", properties->deviceName );
                return false;
            }
            return true;
        }

        return false;
    }

    bool checkDeviceExtensionSupport
    (
        VkPhysicalDevice physical_device,
        const std::vector< const char* >& device_extensions
    )
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties( physical_device, nullptr, &extensionCount, nullptr );

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties( physical_device, nullptr, &extensionCount, availableExtensions.data() );

        std::set<std::string> required_extensions( device_extensions.begin(), device_extensions.end() );

        for (const auto& extension : availableExtensions)
        {
            required_extensions.erase( extension.extensionName );
        }

        return required_extensions.empty();
    }

    bool isPhysicalDeviceSuitable
    (
        VkSurfaceKHR surface,
        VkPhysicalDevice physical_device,
        const std::vector< const char* >& device_extensions
    )
    {
        bool extensions_supported = checkDeviceExtensionSupport( physical_device, device_extensions );

        bool swapchain_adequate = false;
        if (extensions_supported)
        {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> present_modes;

            getPhysicalDeviceSwapChainSupport( surface, physical_device, capabilities, formats, present_modes );
            swapchain_adequate = ( !formats.empty() && !present_modes.empty() );
        }

        VkPhysicalDeviceFeatures supported_features;
        vkGetPhysicalDeviceFeatures( physical_device, &supported_features );

        return extensions_supported && swapchain_adequate && supported_features.samplerAnisotropy;
    }

    VkResult debugUtilsMessengerEXT
    (
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger
    )
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT" );
        if (func != nullptr)
        {
            return func( instance, pCreateInfo, pAllocator, pDebugMessenger );
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void getPhysicalDeviceSwapChainSupport
    (
        VkSurfaceKHR surface,
        VkPhysicalDevice physical_device,
        VkSurfaceCapabilitiesKHR& capabilities,
        std::vector<VkSurfaceFormatKHR>& formats,
        std::vector<VkPresentModeKHR>& present_modes
    )
    {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR( physical_device, surface, &capabilities );

        uint32_t format_count;
        vkGetPhysicalDeviceSurfaceFormatsKHR( physical_device, surface, &format_count, nullptr );

        if (format_count != 0)
        {
            formats.resize( format_count );
            vkGetPhysicalDeviceSurfaceFormatsKHR( physical_device, surface, &format_count, formats.data() );
        }

        uint32_t present_mode_count;
        vkGetPhysicalDeviceSurfacePresentModesKHR( physical_device, surface, &present_mode_count, nullptr);

        if ( present_mode_count != 0 )
        {
            present_modes.resize( present_mode_count );
            vkGetPhysicalDeviceSurfacePresentModesKHR( physical_device, surface, &present_mode_count, present_modes.data() );
        }
    }

    VkSurfaceFormatKHR swapSurfaceFormat
    (
        const std::vector< VkSurfaceFormatKHR >& available_formats
    )
    {
        for ( const auto& available_format : available_formats )
        {
            if ( available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR )
            {
                return available_format;
            }
        }
        return available_formats[0];
    }

    VkPresentModeKHR swapPresentMode
    (
        const std::vector< VkPresentModeKHR >& available_present_modes
    )
    {
        for ( const auto& available_present_mode : available_present_modes )
        {
            if ( available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR )
            {
                return available_present_mode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D swapExtent
    (
        GLFWwindow* window,
        const VkSurfaceCapabilitiesKHR& capabilities
    )
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            int width, height;
            glfwGetFramebufferSize( window, &width, &height );

            VkExtent2D actualExtent =
            {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp
            (
                 actualExtent.width,
                 capabilities.minImageExtent.width,
                 capabilities.maxImageExtent.width
            );

            actualExtent.height = std::clamp
            (
                 actualExtent.height,
                 capabilities.minImageExtent.height,
                 capabilities.maxImageExtent.height
            );

            return actualExtent;
        }
    }

    VkFormat supportedFormat
    (
         VkPhysicalDevice physical_device,
         const std::vector<VkFormat>& candidates,
         VkImageTiling tiling,
         VkFormatFeatureFlags features
    )
    {
        for (VkFormat format : candidates)
        {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties( physical_device, format, &props );

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        throw std::runtime_error("failed to find supported format!");
    }

    VkSemaphore creatorSemaphore( VkDevice device )
    {
        VkSemaphoreCreateInfo semaphore_info{};
        semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkSemaphore semaphore;
        if ( vkCreateSemaphore( device, &semaphore_info, nullptr, &semaphore ) != VK_SUCCESS )
        {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
        return semaphore;
    }

    VkFence createFence( VkDevice device )
    {
        VkFenceCreateInfo fence_info{};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        VkFence fence;
        if ( vkCreateFence( device, &fence_info, nullptr, &fence ) != VK_SUCCESS )
        {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
        return fence;
    }

    void destroySemaphore( VkDevice device, VkSemaphore& semaphore )
    {
        if ( semaphore != VK_NULL_HANDLE )
        {
            vkDestroySemaphore( device, semaphore, nullptr );
            semaphore = VK_NULL_HANDLE;
        }
    }

    void destroyFence( VkDevice device, VkFence& fence )
    {
        if ( fence != VK_NULL_HANDLE )
        {
            vkDestroyFence( device, fence, nullptr );
            fence = VK_NULL_HANDLE;
        }
    }
}}

#define KEGE_GLSL_COMPILER_PATH "/Applications/Developer/VulkanSDK/1.3.236.0/macOS/bin/glslc"
namespace kege{namespace vk{

    bool readfile( std::vector< char >& buffer, const std::string& filename )
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            return false;
        }
        size_t filesize = (size_t)file.tellg();
        buffer.resize(filesize);

        file.seekg(0);
        file.read(buffer.data(), filesize);
        file.close();

        return true;
    }


    std::string getFileEXT( const std::string& filename )
    {
        return filename.substr( filename.find_last_of(".") + 1 );
    }
    std::string getFileName( const std::string& filename )
    {
        return filename.substr( filename.find_first_of(".") + 1 );
    }

    std::string creatSPIRVFileFromGLSLFile( const std::string& filename )
    {
        const std::string exe = KEGE_GLSL_COMPILER_PATH;
        const std::string file = filename.substr( filename.find_last_of("/\\") + 1 );
        const std::string name = file.substr(0, file.rfind("."));
        const std::string ext = getFileEXT( file );// file.substr(0, file.rfind("."));

        // create directory to store SPIR-V files
        size_t last_slash_idx = filename.rfind("/");
        const std::string directory = filename.substr(0, last_slash_idx) + "/spv";
        std::filesystem::create_directory( directory );

        // compile vs file into SPIR-V files and store into directory spv
        const std::string output = directory + "/" + name + "-" + ext + ".spv";
        const std::string command = exe + " " + filename + " -o " + output;
        if( system( command.data() ) != 0 )
        {
            KEGE_ERROR( "something went wrong while converting GLSL file %s to SPIRV file %s", filename.c_str(), output.c_str() );
            throw std::runtime_error("");
            return "";
        }

        return output;
    }

    bool convertGLSLToSPIRVFile( std::vector< char >& shader_bytecode, const std::string& filename )
    {
        const std::string exe = KEGE_GLSL_COMPILER_PATH;
        const std::string file = filename.substr( filename.find_last_of("/\\") + 1 );
        const std::string name = getFileEXT( filename );// file.substr(0, file.rfind("."));

        // create directory to store SPIR-V files
        size_t last_slash_idx = filename.rfind("/");
        const std::string directory = filename.substr(0, last_slash_idx) + "/spv";
        std::filesystem::create_directory( directory );

        // compile vs file into SPIR-V files and store into directory spv
        const std::string spir_v_filename = directory + "/" + name + ".spv";
        const std::string command = exe + " " + filename + " -o " + spir_v_filename;
        if( system( command.data() ) != 0 )
        {
            throw std::runtime_error("");
            return false;;
        }

        return readfile( shader_bytecode, spir_v_filename );
    }

    /**
     load SPIR-V file, or GLSL file. if the @b filename ext is not an  spv, then the file is converted to SPIR-V, loaded, and the content of the file is returned
     @param filename should be an SPIR-V file, or GLSL file.
     @return SPIR-V binary file data
     */
    bool loadShaderSource( std::vector< char >& source, const std::string& filename )
    {
        const std::string& ext = getFileEXT( filename );
        if ( ext == "spv" )
        {
            return readfile( source, filename );
        }
        else
        {
            const std::string source_filename = creatSPIRVFileFromGLSLFile( filename );
            if ( source_filename == "" )
            {
                KEGE_ERROR("failed to compile file -> %s", filename.c_str(), " to SPIR-V !!" );
                return false;
            }
            else
            {
                return readfile( source, source_filename );
            }
        }
        return true;
    }

    VkVertexInputRate getVertexInputRate( core::VertexInputRate::Enum rate )
    {
        switch (rate)
        {
            case core::VertexInputRate::PER_VERTEX: return VK_VERTEX_INPUT_RATE_VERTEX;
            case core::VertexInputRate::PER_INSTANCE: return VK_VERTEX_INPUT_RATE_INSTANCE;;
            default: throw std::runtime_error( "INVALID core::VertexInputRate::Enum" );
        }
    }

    void parseVertexInputAttributes
    (
         VkShaderStageFlagBits shader_type,
         SpvReflectShaderModule& shader_module,
         std::vector< VkVertexInputAttributeDescription >& vertex_attributes,
         std::vector< VkVertexInputBindingDescription >& vertex_bindings
    )
    {

        uint32_t input_count = 0;
        SpvReflectResult result = spvReflectEnumerateInputVariables( &shader_module, &input_count, nullptr );
        if (result != SPV_REFLECT_RESULT_SUCCESS)
        {
            throw std::runtime_error( "spvReflect failed getting total attribute count." );
        }

        std::vector< SpvReflectInterfaceVariable* > attributes;
        attributes.resize( input_count );
        result = spvReflectEnumerateInputVariables( &shader_module, &input_count, attributes.data() );
        if (result != SPV_REFLECT_RESULT_SUCCESS)
        {
            throw std::runtime_error( "spvReflect failed getting attribute info." );
        }

        if( shader_type == VK_SHADER_STAGE_VERTEX_BIT )
        {
            // integrate shader variables
            uint32_t offset = 0;
            for (const auto& var : attributes )
            {
                VkVertexInputAttributeDescription a;
                a.format   = toVkFormat( var->format );
                a.location = var->location;
                a.offset   = offset;
                a.binding  = 0;
                if( a.location != 4294967295 )
                {
                    vertex_attributes.push_back( a );
                    offset += vkFormatSize( a.format );
                }
            }

            VkVertexInputBindingDescription input_binding_description = {};
            input_binding_description.binding = 0;
            input_binding_description.stride = offset;
            input_binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            vertex_bindings.push_back( input_binding_description );
        }
    }

    void parsePushConstantRange
    (
        VkShaderStageFlagBits shader_type,
        SpvReflectShaderModule& shader_module,
        std::vector< VkPushConstantRange >& push_constants
    )
    {
        uint32_t push_constant_count = 0;
        SpvReflectResult result = spvReflectEnumeratePushConstantBlocks( &shader_module, &push_constant_count, NULL );
        if (result != SPV_REFLECT_RESULT_SUCCESS)
        {
            throw std::runtime_error( "spvReflect failed getting total number of push-constants." );
        }

        std::vector< SpvReflectBlockVariable* > spv_push_constants;
        spv_push_constants.resize( push_constant_count );
        result = spvReflectEnumeratePushConstantBlocks( &shader_module, &push_constant_count, spv_push_constants.data() );
        if (result != SPV_REFLECT_RESULT_SUCCESS)
        {
            throw std::runtime_error( "spvReflect failed getting push-constants info." );
        }

        // integrate push constants
        for (uint32_t i = 0; i < spv_push_constants.size(); i++)
        {
            SpvReflectBlockVariable* push_constant_block = spv_push_constants[i];

            VkPushConstantRange constant{};
            constant.size = push_constant_block->size;
            constant.stageFlags = shader_type;
            constant.offset = push_constant_block->offset;
            push_constants.push_back( constant );
        }
    }

//    void parseDescriptorSetLayout
//    (
//        VkShaderStageFlagBits shader_type,
//        SpvReflectShaderModule& shader_module,
//        std::vector< kege::Ref< vk::DescriptorSetLayout > >& descriptor_set_layouts
//    )
//    {
//        uint32_t uniform_count = 0;
//        SpvReflectResult result = spvReflectEnumerateDescriptorBindings( &shader_module, &uniform_count, nullptr );
//        if (result != SPV_REFLECT_RESULT_SUCCESS)
//        {
//            throw std::runtime_error( "spvReflect failed getting totall number of unifrom." );
//        }
//
//        std::vector< SpvReflectDescriptorBinding* > spv_descriptor_binding;
//        spv_descriptor_binding.resize( uniform_count );
//        result = spvReflectEnumerateDescriptorBindings( &shader_module, &uniform_count, spv_descriptor_binding.data() );
//        if (result != SPV_REFLECT_RESULT_SUCCESS)
//        {
//            throw std::runtime_error( "spvReflect failed getting uniform info." );
//        }
//
//        typedef std::pair< kege::string, std::map< uint32_t, VkDescriptorSetLayoutBinding > > BindingMap;
//
//        // integrate descriptor-set-layout
//        std::map< uint32_t, BindingMap > layout;
//        for (const auto& spv_binding : spv_descriptor_binding)
//        {
//            VkDescriptorSetLayoutBinding binding = {};
//            binding.descriptorType = toVkDescriptorType( spv_binding->descriptor_type );
//            binding.descriptorCount = spv_binding->count;
//            binding.binding = spv_binding->binding;
//            binding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT | VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
//            binding.pImmutableSamplers = nullptr;
//
//            BindingMap& dsl = layout[ spv_binding->set ];
//            dsl.second[ binding.binding ] = binding;
//            if( spv_binding->type_description->type_name )
//                dsl.first = spv_binding->type_description->type_name;
//            else
//                dsl.first = spv_binding->name;
//        }
//
//        vk::PipelineLayoutManager* mgr = vk::Device::get()->getPipelineLayoutManager();
//        for (const auto& m : layout)
//        {
//            std::vector< VkDescriptorSetLayoutBinding > bindings;
//            for (const auto& n : m.second.second)
//            {
//                bindings.push_back( n.second );
//            }
//            vk::DescriptorSetLayout* dsl = mgr->createDescriptorSetLayout( m.second.first, bindings );
//            descriptor_set_layouts.push_back( dsl );
//        }
//    }


    VkFormat toVkFormat( SpvReflectFormat format )
    {
        switch ( format )
        {
            default:
            case SPV_REFLECT_FORMAT_UNDEFINED: return VK_FORMAT_UNDEFINED;
            case SPV_REFLECT_FORMAT_R32_UINT: return VK_FORMAT_R32_UINT;
            case SPV_REFLECT_FORMAT_R32_SINT: return VK_FORMAT_R32_SINT;
            case SPV_REFLECT_FORMAT_R32_SFLOAT: return VK_FORMAT_R32_SFLOAT;
            case SPV_REFLECT_FORMAT_R32G32_UINT: return VK_FORMAT_R32G32_UINT;
            case SPV_REFLECT_FORMAT_R32G32_SINT: return VK_FORMAT_R32G32_SINT;
            case SPV_REFLECT_FORMAT_R32G32_SFLOAT : return VK_FORMAT_R32G32_SFLOAT;
            case SPV_REFLECT_FORMAT_R32G32B32_UINT: return VK_FORMAT_R32G32B32_UINT;
            case SPV_REFLECT_FORMAT_R32G32B32_SINT: return VK_FORMAT_R32G32B32_SINT;
            case SPV_REFLECT_FORMAT_R32G32B32_SFLOAT: return VK_FORMAT_R32G32B32_SFLOAT;
            case SPV_REFLECT_FORMAT_R32G32B32A32_UINT: return VK_FORMAT_R32G32B32A32_UINT;
            case SPV_REFLECT_FORMAT_R32G32B32A32_SINT: return VK_FORMAT_R32G32B32A32_SINT;
            case SPV_REFLECT_FORMAT_R32G32B32A32_SFLOAT: return VK_FORMAT_R32G32B32A32_SFLOAT;
            case SPV_REFLECT_FORMAT_R64_UINT: return VK_FORMAT_R64_UINT;
            case SPV_REFLECT_FORMAT_R64_SINT: return VK_FORMAT_R64_SINT;
            case SPV_REFLECT_FORMAT_R64_SFLOAT: return VK_FORMAT_R64_SFLOAT;
            case SPV_REFLECT_FORMAT_R64G64_UINT: return VK_FORMAT_R64G64_UINT;
            case SPV_REFLECT_FORMAT_R64G64_SINT: return VK_FORMAT_R64G64_SINT;
            case SPV_REFLECT_FORMAT_R64G64_SFLOAT: return VK_FORMAT_R64G64_SFLOAT;
            case SPV_REFLECT_FORMAT_R64G64B64_UINT: return VK_FORMAT_R64G64B64_UINT;
            case SPV_REFLECT_FORMAT_R64G64B64_SINT: return VK_FORMAT_R64G64B64_SINT;
            case SPV_REFLECT_FORMAT_R64G64B64_SFLOAT: return VK_FORMAT_R64G64B64_SFLOAT;
            case SPV_REFLECT_FORMAT_R64G64B64A64_UINT: return VK_FORMAT_R64G64B64A64_UINT;
            case SPV_REFLECT_FORMAT_R64G64B64A64_SINT: return VK_FORMAT_R64G64B64A64_SINT;
            case SPV_REFLECT_FORMAT_R64G64B64A64_SFLOAT: return VK_FORMAT_R64G64B64A64_SFLOAT;
        }
    }

    VkDescriptorType toVkDescriptorType( SpvReflectDescriptorType descriptor_type )
    {
        VkDescriptorType type;
        switch ( descriptor_type )
        {
            case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
                type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                break;

            case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                break;

            case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE:
                type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                break;

            case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER:
                type = VK_DESCRIPTOR_TYPE_SAMPLER;
                break;

            case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
                type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                break;

            case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
                type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
                break;

            case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
                type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
                break;

            case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER:
                type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                break;

            case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
                type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
                break;

            case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
                type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
                break;

            case SPV_REFLECT_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
                type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
                break;

            case SPV_REFLECT_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR:
                type = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
                break;

            default: KEGE_FATAL("UNKNOW SpvReflectDescriptorType");
                break;
        }
        return type;
    }


    VkPolygonMode getPolygonMode( core::Polygon::Mode mode )
    {
        switch( mode )
        {
        case core::Polygon::LINE: return VK_POLYGON_MODE_LINE;
            break;

        case core::Polygon::POINT: return VK_POLYGON_MODE_POINT;
            break;

        default: return VK_POLYGON_MODE_FILL;
            break;
        }
    }

    VkPrimitiveTopology getPrimitiveTopology( core::Primitive::Type topology )
    {
        switch ( topology )
        {
            default:
            case core::Primitive::TRIANGLE_LIST: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; break;
            case core::Primitive::TRIANGLE_FAN: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN; break;
            case core::Primitive::TRIANGLE_STRIP: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP; break;
            case core::Primitive::TRIANGLE_LIST_WITH_ADJACENCY: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY; break;
            case core::Primitive::TRIANGLE_STRIP_WITH_ADJACENCY: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY; break;
            case core::Primitive::LINE_LIST: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST; break;
            case core::Primitive::LINE_STRIP: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP; break;
            case core::Primitive::LINE_LIST_WITH_ADJACENCY: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY; break;
            case core::Primitive::LINE_STRIP_WITH_ADJACENCY:  return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY; break;
            case core::Primitive::POINT_LIST: return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST; break;
            case core::Primitive::PATCH_LIST: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST; break;
        }
    }

    size_t vkFormatSize(VkFormat format)
    {
        switch (format)
        {
        case VK_FORMAT_R64_UINT:
        case VK_FORMAT_R64_SINT:
        case VK_FORMAT_R64_SFLOAT:
            return sizeof(uint64_t);

        case VK_FORMAT_R64G64_UINT:
        case VK_FORMAT_R64G64_SINT:
        case VK_FORMAT_R64G64_SFLOAT:
            return sizeof(uint64_t)*2;


        case VK_FORMAT_R32G32_UINT:
        case VK_FORMAT_R32G32_SINT:
        case VK_FORMAT_R32G32_SFLOAT:
            return sizeof(uint32_t)*2;

        case VK_FORMAT_R64G64B64_UINT:
        case VK_FORMAT_R64G64B64_SINT:
        case VK_FORMAT_R64G64B64_SFLOAT:
            return sizeof(uint64_t)*3;

        case VK_FORMAT_R32G32B32_UINT:
        case VK_FORMAT_R32G32B32_SINT:
        case VK_FORMAT_R32G32B32_SFLOAT:
            return sizeof(uint32_t)*3;

        case VK_FORMAT_R16G16B16_UINT:
        case VK_FORMAT_R16G16B16_SINT:
        case VK_FORMAT_R16G16B16_SFLOAT:
            return sizeof(uint16_t)*3;

        case VK_FORMAT_R64G64B64A64_UINT:
        case VK_FORMAT_R64G64B64A64_SFLOAT:
            return sizeof(uint64_t)*4;

        case VK_FORMAT_R32G32B32A32_UINT:
        case VK_FORMAT_R32G32B32A32_SFLOAT:
            return sizeof(uint32_t)*4;

        case VK_FORMAT_R16G16B16A16_UINT:
        case VK_FORMAT_R16G16B16A16_SFLOAT:
            return sizeof(uint16_t)*4;

        case VK_FORMAT_R4G4_UNORM_PACK8:
        case VK_FORMAT_R8_UNORM:
        case VK_FORMAT_R8_SNORM:
        case VK_FORMAT_R8_USCALED:
        case VK_FORMAT_R8_SSCALED:
        case VK_FORMAT_R8_UINT:
        case VK_FORMAT_R8_SINT:
        case VK_FORMAT_S8_UINT:
            return sizeof(uint8_t);

        case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
        case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
        case VK_FORMAT_R5G6B5_UNORM_PACK16:
        case VK_FORMAT_B5G6R5_UNORM_PACK16:
        case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
        case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
        case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
        case VK_FORMAT_R8G8_UNORM:
        case VK_FORMAT_R8G8_SNORM:
        case VK_FORMAT_R8G8_USCALED:
        case VK_FORMAT_R8G8_SSCALED:
        case VK_FORMAT_R8G8_UINT:
        case VK_FORMAT_R8G8_SINT:
        case VK_FORMAT_R16_UNORM:
        case VK_FORMAT_R16_SNORM:
        case VK_FORMAT_R16_USCALED:
        case VK_FORMAT_R16_SSCALED:
        case VK_FORMAT_R16_UINT:
        case VK_FORMAT_R16_SINT:
        case VK_FORMAT_R16_SFLOAT:
        case VK_FORMAT_D16_UNORM:
            return sizeof(uint16_t);

        case VK_FORMAT_R8G8B8_UNORM:
        case VK_FORMAT_R8G8B8_SNORM:
        case VK_FORMAT_R8G8B8_USCALED:
        case VK_FORMAT_R8G8B8_SSCALED:
        case VK_FORMAT_R8G8B8_UINT:
        case VK_FORMAT_R8G8B8_SINT:
        case VK_FORMAT_B8G8R8_UNORM:
        case VK_FORMAT_B8G8R8_SNORM:
        case VK_FORMAT_B8G8R8_USCALED:
        case VK_FORMAT_B8G8R8_SSCALED:
        case VK_FORMAT_B8G8R8_UINT:
        case VK_FORMAT_B8G8R8_SINT:
        case VK_FORMAT_R8G8B8A8_UNORM:
        case VK_FORMAT_R8G8B8A8_SNORM:
        case VK_FORMAT_R8G8B8A8_USCALED:
        case VK_FORMAT_R8G8B8A8_SSCALED:
        case VK_FORMAT_R8G8B8A8_UINT:
        case VK_FORMAT_R8G8B8A8_SINT:
        case VK_FORMAT_B8G8R8A8_UNORM:
        case VK_FORMAT_B8G8R8A8_SNORM:
        case VK_FORMAT_B8G8R8A8_USCALED:
        case VK_FORMAT_B8G8R8A8_SSCALED:
        case VK_FORMAT_B8G8R8A8_UINT:
        case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
        case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
        case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
        case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
        case VK_FORMAT_A8B8G8R8_UINT_PACK32:
        case VK_FORMAT_A8B8G8R8_SINT_PACK32:
        case VK_FORMAT_B8G8R8A8_SRGB:
        case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
        case VK_FORMAT_B8G8R8A8_SINT:
        case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
        case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
        case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
        case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
        case VK_FORMAT_A2R10G10B10_UINT_PACK32:
        case VK_FORMAT_A2R10G10B10_SINT_PACK32:
        case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
        case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
        case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
        case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
        case VK_FORMAT_A2B10G10R10_UINT_PACK32:
        case VK_FORMAT_A2B10G10R10_SINT_PACK32:
        case VK_FORMAT_R16G16_UNORM:
        case VK_FORMAT_R16G16_SNORM:
        case VK_FORMAT_R16G16_USCALED:
        case VK_FORMAT_R16G16_SSCALED:
        case VK_FORMAT_R16G16_UINT:
        case VK_FORMAT_R16G16_SINT:
        case VK_FORMAT_R16G16_SFLOAT:
        case VK_FORMAT_R32_UINT:
        case VK_FORMAT_R32_SINT:
        case VK_FORMAT_R32_SFLOAT:
        case VK_FORMAT_D32_SFLOAT:
        case VK_FORMAT_D32_SFLOAT_S8_UINT:
            return sizeof(uint32_t);

        case VK_FORMAT_UNDEFINED:
        default: return 0;
        }
    }

    VkFormat getVkFormat( core::ImgFormat format )
    {
        switch ( format )
        {
            default:                return VK_FORMAT_UNDEFINED;
                
            case core::D16:         return VK_FORMAT_D16_UNORM;
            case core::D24:         return VK_FORMAT_D24_UNORM_S8_UINT;
            case core::D32:         return VK_FORMAT_D32_SFLOAT;

            case core::R8_SRGB:     return VK_FORMAT_R8_SRGB;
            case core::RG8_SRGB:    return VK_FORMAT_R8G8_SRGB;
            case core::RGB8_SRGB:   return VK_FORMAT_R8G8B8_SRGB;
            case core::RGBA8_SRGB:  return VK_FORMAT_R8G8B8A8_SRGB;

            case core::BGR8_SRGB:   return VK_FORMAT_B8G8R8_SRGB;
            case core::BGRA8_SRGB:  return VK_FORMAT_B8G8R8A8_SRGB;

            case core::R8_UNORM:    return VK_FORMAT_R8_UNORM;
            case core::RG8_UNORM:   return VK_FORMAT_R8G8_UNORM;
            case core::RGB8_UNORM:  return VK_FORMAT_R8G8B8_UNORM;
            case core::RGBA8_UNORM: return VK_FORMAT_R8G8B8A8_UNORM;

            case core::R8_UINT:     return VK_FORMAT_R8_UINT;
            case core::RG8_UINT:    return VK_FORMAT_R8G8_UINT;
            case core::RGB8_UINT:   return VK_FORMAT_R8G8B8_UINT;
            case core::RGBA8_UINT:  return VK_FORMAT_R8G8B8A8_UINT;

            case core::R16_UINT:    return VK_FORMAT_R16_UINT;
            case core::RG16_UINT:   return VK_FORMAT_R16G16_UINT;
            case core::RGB16_UINT:  return VK_FORMAT_R16G16B16_UINT;
            case core::RGBA16_UINT: return VK_FORMAT_R16G16B16A16_UINT;

            case core::R32_UINT:    return VK_FORMAT_R32_UINT;
            case core::RG32_UINT:   return VK_FORMAT_R32G32_UINT;
            case core::RGB32_UINT:  return VK_FORMAT_R32G32B32_UINT;
            case core::RGBA32_UINT: return VK_FORMAT_R32G32B32A32_UINT;

            case core::R64_UINT:    return VK_FORMAT_R64_UINT;
            case core::RG64_UINT:   return VK_FORMAT_R64G64_UINT;
            case core::RGB64_UINT:  return VK_FORMAT_R64G64B64_UINT;
            case core::RGBA64_UINT: return VK_FORMAT_R64G64B64A64_UINT;

            case core::R8_SINT:     return VK_FORMAT_R8_SINT;
            case core::RG8_SINT:    return VK_FORMAT_R8G8_SINT;
            case core::RGB8_SINT:   return VK_FORMAT_R8G8B8_SINT;
            case core::RGBA8_SINT:  return VK_FORMAT_R8G8B8A8_SINT;

            case core::R16_SINT:    return VK_FORMAT_R16_SINT;
            case core::RG16_SINT:   return VK_FORMAT_R16G16_SINT;
            case core::RGB16_SINT:  return VK_FORMAT_R16G16B16_SINT;
            case core::RGBA16_SINT: return VK_FORMAT_R16G16B16A16_SINT;

            case core::R32_SINT:    return VK_FORMAT_R32_SINT;
            case core::RG32_SINT:   return VK_FORMAT_R32G32_SINT;
            case core::RGB32_SINT:  return VK_FORMAT_R32G32B32_SINT;
            case core::RGBA32_SINT: return VK_FORMAT_R32G32B32A32_SINT;

            case core::R64_SINT:    return VK_FORMAT_R64_SINT;
            case core::RG64_SINT:   return VK_FORMAT_R64G64_SINT;
            case core::RGB64_SINT:  return VK_FORMAT_R64G64B64_SINT;
            case core::RGBA64_SINT: return VK_FORMAT_R64G64B64A64_SINT;

            case core::R32_FLOAT:    return VK_FORMAT_R32_SFLOAT;
            case core::RG32_FLOAT:   return VK_FORMAT_R32G32_SFLOAT;
            case core::RGB32_FLOAT:  return VK_FORMAT_R32G32B32_SFLOAT;
            case core::RGBA32_FLOAT: return VK_FORMAT_R32G32B32A32_SFLOAT;

            case core::R64_FLOAT:    return VK_FORMAT_R64_SFLOAT;
            case core::RG64_FLOAT:   return VK_FORMAT_R64G64_SFLOAT;
            case core::RGB64_FLOAT:  return VK_FORMAT_R64G64B64_SFLOAT;
            case core::RGBA64_FLOAT: return VK_FORMAT_R64G64B64A64_SFLOAT;
        }
    }
    
    VkFilter getFilter( core::Sampler::Filter filter )
    {
        switch ( filter )
        {
            case core::Sampler::LINEAR:  return VK_FILTER_LINEAR;
            case core::Sampler::NEAREST: return VK_FILTER_NEAREST;
            case core::Sampler::CUBIC:   return VK_FILTER_CUBIC_IMG;
            default: break;
        }
        return VK_FILTER_LINEAR;
    }

    VkSamplerAddressMode getSamplerAddressMode( core::Sampler::Wrapping wrap )
    {
        switch ( wrap )
        {
            case core::Sampler::REPEAT:                return VK_SAMPLER_ADDRESS_MODE_REPEAT;
            case core::Sampler::MIRRORED_REPEAT:       return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
            case core::Sampler::CLAMP_TO_EDGE:         return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            case core::Sampler::CLAMP_TO_BORDER:       return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
            case core::Sampler::MIRROR_CLAMP_TO_EDGE:  return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
            default: break;
        }
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    }

}}

namespace kege{namespace vk{

    const char* vkGetError( VkResult result )
    {
        switch ( result )
        {
        default: return "";

        case VK_ERROR_OUT_OF_POOL_MEMORY:
            return "VK_ERROR_OUT_OF_POOL_MEMORY";
        case VK_NOT_READY:
            return "VK_NOT_READY";
        case VK_TIMEOUT:
            return "VK_TIMEOUT";
        case VK_EVENT_SET:
            return "VK_EVENT_SET";
        case VK_EVENT_RESET:
            return "VK_EVENT_RESET";
        case VK_INCOMPLETE:
            return "VK_INCOMPLETE";
        case VK_PIPELINE_COMPILE_REQUIRED:
            return "VK_PIPELINE_COMPILE_REQUIRED";
        case VK_THREAD_IDLE_KHR:
            return "VK_THREAD_IDLE_KHR";
        case VK_THREAD_DONE_KHR:
            return "VK_THREAD_DONE_KHR";
        case VK_OPERATION_DEFERRED_KHR:
            return "VK_OPERATION_DEFERRED_KHR";
        case VK_OPERATION_NOT_DEFERRED_KHR:
            return "VK_OPERATION_NOT_DEFERRED_KHR";
        case VK_ERROR_INITIALIZATION_FAILED:
            return "VK_ERROR_INITIALIZATION_FAILED";
        case VK_ERROR_DEVICE_LOST:
            return "VK_ERROR_DEVICE_LOST";
        case VK_ERROR_MEMORY_MAP_FAILED:
            return "VK_ERROR_MEMORY_MAP_FAILED";
        case VK_ERROR_LAYER_NOT_PRESENT:
            return "VK_ERROR_LAYER_NOT_PRESENT";
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return "VK_ERROR_EXTENSION_NOT_PRESENT";
        case VK_ERROR_FEATURE_NOT_PRESENT:
            return "VK_ERROR_FEATURE_NOT_PRESENT";
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return "VK_ERROR_INCOMPATIBLE_DRIVER";
        case VK_ERROR_TOO_MANY_OBJECTS:
            return "VK_ERROR_TOO_MANY_OBJECTS";
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            return "VK_ERROR_FORMAT_NOT_SUPPORTED";
        case VK_ERROR_UNKNOWN:
            return "VK_ERROR_UNKNOWN";
        case VK_ERROR_INVALID_EXTERNAL_HANDLE:
            return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
        case VK_ERROR_FRAGMENTATION:
            return "VK_ERROR_FRAGMENTATION";
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
            return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
        case VK_ERROR_SURFACE_LOST_KHR:
            return "VK_ERROR_SURFACE_LOST_KHR";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
        case VK_SUBOPTIMAL_KHR:
            return "VK_SUBOPTIMAL_KHR";
        case VK_ERROR_OUT_OF_DATE_KHR:
            return "VK_ERROR_OUT_OF_DATE_KHR";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
        case VK_ERROR_VALIDATION_FAILED_EXT:
            return "VK_ERROR_VALIDATION_FAILED_EXT";
        case VK_ERROR_INVALID_SHADER_NV:
            return "VK_ERROR_INVALID_SHADER_NV";
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
            return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
        case VK_ERROR_NOT_PERMITTED_KHR:
            return "VK_ERROR_NOT_PERMITTED_KHR";
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
            return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
        case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
            return "VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
        }
    }
}}
