//
//  Buffer.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 2/3/23.
//

#ifndef KEGE_VULKAN_BUFFER
#define KEGE_VULKAN_BUFFER
#include "../../device/core/Buffer.hpp"
#include "../../device/vulkan/utils.hpp"
namespace kege{namespace vk{
    class Device;
    

    class Buffer : public kege::core::Buffer
    {
    public:

        VkResult operator()
        (
            const VkBufferCreateInfo& create_info,
            const void* data,
            VkMemoryPropertyFlags memory_property
        );

        VkResult construct( core::Buffer::Type type, uint64_t size, const void* data, core::Buffer::Usage usage );
        void destruct();

        void copyFromSystemMemory( uint64_t offset, uint64_t size, void*& data );
        void copyFromSystemMemory( uint64_t size, void*& data );
        void copyFromSystemMemory( void*& data );

        void copyToSystemMemory( uint64_t offset, uint64_t size, const void* data );
        void copyToSystemMemory( uint64_t size, const void* data );

        VkResult stageBuffer( VkDeviceSize size, const void* data );
        
        const VkDeviceMemory memory()const;
        const VkBuffer handle()const;
        uint64_t sizeInBytes()const;

        core::Buffer::Type bufferType()const;
        const vk::Buffer* vulkan()const;
        vk::Buffer* vulkan();

        const void* mapMemory()const;
        void unmapMemory();
        void* mapMemory();

        Buffer
        (
            VkBufferUsageFlags usage,
            VkDeviceSize size,
            const void* data,
            VkMemoryPropertyFlags memory_property
        );
        ~Buffer();
        Buffer();

    private:

        static int instances;
        core::Buffer::Type _buffer_type;
        VkDeviceMemory _memory;
        VkDeviceSize _size;
        VkBuffer _handle;
    };
}}
#endif /* KEGE_VULKAN_BUFFER_HPP */
