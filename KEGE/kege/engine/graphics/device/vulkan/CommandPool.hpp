//
//  CommandPool.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/21/23.
//

#ifndef CommandPool_hpp
#define CommandPool_hpp

#include "../vulkan/utils.hpp"

namespace kege{namespace vk{

    class CommandPool
    {
    public:

        VkResult construct( VkCommandPoolCreateFlags flags, core::CmdQueueType queue_index );
        VkCommandBuffer allocateCommandBuffers( VkCommandBufferLevel level );
        void freeCommandBuffer( VkCommandBuffer& command );
        core::CmdQueueType cmdQueueType()const;
        const VkCommandPool handle()const;
        void destruct();
        CommandPool();

    private:

        core::CmdQueueType _queue_type;
        VkCommandPool _handle;
    };

}}
#endif /* CommandPool_hpp */
