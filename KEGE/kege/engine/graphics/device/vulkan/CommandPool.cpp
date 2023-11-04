//
//  CommandPool.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/21/23.
//

#include "CommandPool.hpp"
#include "../vulkan/Device.hpp"

namespace kege{namespace vk{

    VkResult CommandPool::construct( VkCommandPoolCreateFlags flags, core::CmdQueueType queue_index )
    {
        _queue_type = queue_index;
        uint32_t queue_family_index = vk::Device::get()->getDeviceQueueFamily()[ queue_index ].value();

        VkCommandPoolCreateInfo pool_info{};
        pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        pool_info.queueFamilyIndex = queue_family_index;
        return vkCreateCommandPool( vk::Device::get()->logicalDevice(), &pool_info, nullptr, &_handle);
    }

    VkCommandBuffer CommandPool::allocateCommandBuffers( VkCommandBufferLevel level )
    {
        VkCommandBufferAllocateInfo alloc_enfo{};
        alloc_enfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_enfo.level = level;
        alloc_enfo.commandPool = _handle;
        alloc_enfo.commandBufferCount = 1;

        VkCommandBuffer buffer;
        VkResult result = vkAllocateCommandBuffers( vk::Device::get()->logicalDevice(), &alloc_enfo, &buffer );
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR( vkGetError( result ) );
            return VK_NULL_HANDLE;
        }
        return buffer;
    }

    void CommandPool::freeCommandBuffer( VkCommandBuffer& command )
    {}

    core::CmdQueueType CommandPool::cmdQueueType()const
    {
        return _queue_type;
    }
    
    const VkCommandPool CommandPool::handle()const
    {
        return _handle;
    }

    void CommandPool::destruct()
    {
        if ( _handle != VK_NULL_HANDLE )
        {
            vkDestroyCommandPool( vk::Device::get()->logicalDevice(), _handle, nullptr );
            _handle = VK_NULL_HANDLE;
        }
    }
    
    CommandPool::CommandPool()
    :   _handle( VK_NULL_HANDLE )
    {}

}}
