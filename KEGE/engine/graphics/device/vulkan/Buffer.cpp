//
//  Buffer.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 2/3/23.
//
#include "../../device/vulkan/Device.hpp"
#include "../../device/vulkan/Buffer.hpp"
namespace kege{namespace vk{

    int Buffer::instances = 0;

    VkResult Buffer::operator()
    (
        const VkBufferCreateInfo& create_info,
        const void* data,
        VkMemoryPropertyFlags memory_properties
    )
    {
        VkResult result = vkCreateBuffer( vk::Device::get()->logicalDevice(), &create_info, nullptr, &_handle );
        if ( result != VK_SUCCESS )
        {
            KEGE_FATAL("%s: failed to create buffer!", vkGetError(result) );
            return result;
        }

        VkMemoryRequirements memory_requirements;
        vkGetBufferMemoryRequirements( vk::Device::get()->logicalDevice(), _handle, &memory_requirements );
        uint32_t memory_type_index = getPhysicalDeviceMemoryTypeIndex
        (
            memory_requirements, memory_properties
        );

        VkMemoryAllocateInfo memory_allocate_info =
        {
            VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            nullptr,
            memory_requirements.size,
            memory_type_index
        };

        result = vkAllocateMemory( vk::Device::get()->logicalDevice(), &memory_allocate_info, nullptr, &_memory );
        if( result != VK_SUCCESS )
        {
            KEGE_ERROR( "Could not allocate memory for a buffer." );
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        result = vkBindBufferMemory( vk::Device::get()->logicalDevice(), _handle, _memory, 0 );
        if( result != VK_SUCCESS )
        {
            KEGE_ERROR( "Could not bind memory object to an image." );
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        if( data )
        {
            void* mapmem = nullptr;
            vkMapMemory( vk::Device::get()->logicalDevice(), _memory, 0, create_info.size, 0, &mapmem );
            memcpy( mapmem, data, create_info.size );
            vkUnmapMemory( vk::Device::get()->logicalDevice(), _memory );
        }
        _size = create_info.size;
        return result;
    }

    VkResult Buffer::stageBuffer( VkDeviceSize size, const void* data )
    {
        VkBufferCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.size = size;
        return operator()( create_info, data, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );
    }

    VkResult Buffer::construct( core::Buffer::Type type, uint64_t size, const void* data, core::Buffer::Usage usage )
    {
        _buffer_type = type;
        VkResult result = VK_ERROR_INITIALIZATION_FAILED;

        VkBufferCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.size = size;

        switch ( type )
        {
            case core::Buffer::VERTEX:   create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;  break;
            case core::Buffer::INDEX:    create_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;   break;
            case core::Buffer::UNIFORM:  create_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT; break;
            case core::Buffer::STORAGE:  create_info.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT; break;
            case core::Buffer::TRANSFER: create_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;   break;
            case core::Buffer::UNIFORM_TEXEL: create_info.usage = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;   break;
            case core::Buffer::STORAGE_TEXEL: create_info.usage = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;   break;
            default:break;
        }

        switch ( usage )
        {
            case core::Buffer::STATIC:
            {
                vk::Buffer source;
                result = source.stageBuffer( size, data );

                if( result == VK_SUCCESS )
                {
                    create_info.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
                    result = operator()( create_info, nullptr, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );
                    if( result == VK_SUCCESS )
                    {
                        vk::Device::get()->copyBuffer( *this, source );
                    }
                }
                break;
            }

            case core::Buffer::STAGE:
            case core::Buffer::STREAM:
            case core::Buffer::DYNAMIC:
            {
                result = operator()( create_info, data, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
                break;
            }

            default: break;
        }
        return result;
    }

    void Buffer::destruct()
    {
        vk::Device::get()->waitIdle();
        if ( _memory != VK_NULL_HANDLE )
        {
            vkFreeMemory( vk::Device::get()->logicalDevice(), _memory, nullptr );
            _memory = VK_NULL_HANDLE;
        }

        if ( _handle != VK_NULL_HANDLE )
        {
            vkDestroyBuffer( vk::Device::get()->logicalDevice(), _handle, nullptr );
            _handle = VK_NULL_HANDLE;
        }
    }

    void Buffer::copyFromSystemMemory( uint64_t offset, uint64_t size, void*& data )
    {
        void* mapmem = nullptr;
        vkMapMemory( vk::Device::get()->logicalDevice(), _memory, offset, size, 0, &mapmem );
        memcpy( data, mapmem, size );
        vkUnmapMemory( vk::Device::get()->logicalDevice(), _memory );
    }

    void Buffer::copyFromSystemMemory( uint64_t size, void*& data )
    {
        void* mapmem = nullptr;
        vkMapMemory( vk::Device::get()->logicalDevice(), _memory, 0, size, 0, &mapmem );
        memcpy( data, mapmem, size );
        vkUnmapMemory( vk::Device::get()->logicalDevice(), _memory );
    }

    void Buffer::copyFromSystemMemory( void*& data )
    {
        void* mapmem = mapMemory();
        memcpy( data, mapmem, _size );
        unmapMemory();
    }

    void Buffer::copyToSystemMemory( uint64_t offset, uint64_t size, const void* data )
    {
        void* mapmem = nullptr;
        vkMapMemory( vk::Device::get()->logicalDevice(), _memory, offset, size, 0, &mapmem );
        memcpy( mapmem, data, size );
        vkUnmapMemory( vk::Device::get()->logicalDevice(), _memory );
    }

    void Buffer::copyToSystemMemory( uint64_t size, const void* data )
    {
        void* mapmem = mapMemory();
        memcpy( mapmem, data, size );
        unmapMemory();
    }

    core::Buffer::Type Buffer::bufferType()const
    {
        return _buffer_type;
    }
    
    const vk::Buffer* Buffer::vulkan()const
    {
        return this;
    }
    
    vk::Buffer* Buffer::vulkan()
    {
        return this;
    }

    const void* Buffer::mapMemory()const
    {
        void* mapmem = nullptr;
        vkMapMemory( vk::Device::get()->logicalDevice(), _memory, 0, _size, 0, &mapmem );
        return mapmem;
    }

    void* Buffer::mapMemory()
    {
        void* mapmem = nullptr;
        vkMapMemory( vk::Device::get()->logicalDevice(), _memory, 0, _size, 0, &mapmem );
        return mapmem;
    }

    void Buffer::unmapMemory()
    {
        vkUnmapMemory( vk::Device::get()->logicalDevice(), _memory );
    }

    const VkDeviceMemory Buffer::memory()const
    {
        return _memory;
    }
    
    const VkBuffer Buffer::handle()const
    {
        return _handle;
    }

    uint64_t Buffer::sizeInBytes()const
    {
        return _size;
    }

    Buffer::~Buffer()
    {
        destruct();
    }
    Buffer::Buffer()
    :   _memory(VK_NULL_HANDLE)
    ,   _handle(VK_NULL_HANDLE)
    ,   _size(0)
    {
    }
}}


