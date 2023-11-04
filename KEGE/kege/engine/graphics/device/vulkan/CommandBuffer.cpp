//
//  CommandBuffer.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/2/23.
//

#include "../../device/debug/kege-logger.hpp"
#include "../../device/vulkan/Buffer.hpp"
#include "../../device/vulkan/Device.hpp"
#include "../../device/vulkan/RenderPass.hpp"
#include "../../device/vulkan/Framebuffer.hpp"
#include "../../device/vulkan/CommandPool.hpp"
#include "../../device/vulkan/CommandBuffer.hpp"

namespace kege{namespace vk{

    void CommandBuffer::executeCommands( const std::vector< core::CommandBuffer* >& command_buffers )
    {
        VkCommandBuffer commands[ command_buffers.size() ];
        for (int i=0; i<command_buffers.size(); i++ )
        {
            commands[i] = command_buffers[i]->vulkan()->handle();
        }
        vkCmdExecuteCommands( _handle, static_cast<uint32_t>( command_buffers.size() ), commands );
    }

    void CommandBuffer::drawIndexed
    (   uint32_t index_count
     ,  uint32_t instance_count
     ,  uint32_t first_index
     ,  int32_t vertex_offset
     ,  uint32_t first_instance )
    {
        vkCmdDrawIndexed( _handle, index_count, instance_count, first_index, vertex_offset, first_instance );
    }

    void CommandBuffer::draw
    (   uint32_t vertex_count
     ,  uint32_t instance_count
     ,  uint32_t first_vertex
     ,  uint32_t first_instance )
    {
        vkCmdDraw( _handle, vertex_count, instance_count, first_vertex, first_instance );
    }

    void CommandBuffer::bindUniformLayout( uint32_t first_set, const core::UniformLayout* layout, vk::Shader* shader )
    {
        const VkDescriptorSet descriptors[] = { layout->getUniformHandle()->vulkan()->descriptorSet() };

        vkCmdBindDescriptorSets
        (
            _handle,
            shader->pipelineLayout()->bindPoint(),
            shader->pipelineLayout()->handle(),
            first_set, 1, descriptors,
            0, nullptr
        );
    }

    void CommandBuffer::bindUniformLayout( uint32_t first_set, const core::UniformLayout* layout )
    {
        if ( !layout->getUniformHandle() )
        {
            vk::PipelineLayoutManager* mngr = vk::Device::get()->getPipelineLayoutManager();
            layout->setUniformHandle( mngr->createUniformHandle( layout ) );
        }

        const vk::UniformHandle* uniform = layout->getUniformHandle()->vulkan();
        const VkDescriptorSet descriptors[] = { uniform->descriptorSet() };
        if ( uniform->vulkan()->getPipelineLayoutSet() )
        {
            for (const vk::PipelineLayout* pipeline_layout : *uniform->vulkan()->getPipelineLayoutSet() )
            {
                vkCmdBindDescriptorSets
                (
                    _handle,
                    pipeline_layout->bindPoint(),
                    pipeline_layout->handle(),
                    first_set, 1, descriptors,
                    0, nullptr
                );
            }
        }
    }

    kege::Ref< core::CommandBuffer > CommandBuffer::createSecondaryCommandBuffer()
    {
        VkCommandBuffer handle = _command_pool->allocateCommandBuffers( VK_COMMAND_BUFFER_LEVEL_SECONDARY );
        vk::CommandBuffer* command = new vk::CommandBuffer( _command_pool, handle, this );
        return command;
    }

    void CommandBuffer::beginRenderpass( const core::Framebuffer* framebuffer, core::SubpassContents contents )
    {
        VkClearValue clear_values[2]
        {
            {0.0f, 0.0f, 0.0f, 1.0f},
            {1.0f, 0}
        };
        
        _framebuffer = framebuffer->vulkan();
        _renderpass = _framebuffer->renderpass();

        VkRenderPassBeginInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderArea.extent = {_framebuffer->width(), _framebuffer->height()};
        render_pass_info.renderArea.offset = {0, 0};
        render_pass_info.framebuffer = _framebuffer->handle();
        render_pass_info.renderPass = _renderpass->handle();
        render_pass_info.pClearValues = clear_values;
        render_pass_info.clearValueCount = 2;

        VkSubpassContents subpass_contents = 
        {
            (contents == core::SUBPASS_CONTENTS_PRIMARY_COMMAND_BUFFERS) ?
            VK_SUBPASS_CONTENTS_INLINE :
            VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS
        };
        vkCmdBeginRenderPass( _handle, &render_pass_info, subpass_contents );
        //clearAttachments( framebuffer );
    }

    void CommandBuffer::setImageLayout
    (
     VkImage image,
     VkImageAspectFlags aspects,
     VkImageLayout old_layout,
     VkImageLayout new_layout
    )
    {
        VkImageMemoryBarrier image_barrier = {};
        image_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        image_barrier.pNext = NULL;
        image_barrier.oldLayout = old_layout;
        image_barrier.newLayout = new_layout;
        image_barrier.image = image;
        image_barrier.subresourceRange.aspectMask = aspects;
        image_barrier.subresourceRange.baseMipLevel = 0;
        image_barrier.subresourceRange.levelCount = 1;
        image_barrier.subresourceRange.layerCount = 1;

        switch ( old_layout )
        {
            case VK_IMAGE_LAYOUT_PREINITIALIZED:
                image_barrier.srcAccessMask =
                VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
                break;
            case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
                image_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                break;
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
                image_barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                break;
            case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
                image_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                break;
            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
                image_barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                break;
            default:break;
        }

        switch ( new_layout )
        {
            case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
                image_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                break;
            case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
                image_barrier.srcAccessMask |= VK_ACCESS_TRANSFER_READ_BIT;
                image_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                break;
            case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
                image_barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                image_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                break;
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
                image_barrier.dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                break;
            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
                image_barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
                image_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                break;
            default:break;
        }

        VkPipelineStageFlagBits src_flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        VkPipelineStageFlagBits dst_flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        vkCmdPipelineBarrier( _handle, src_flags, dst_flags, 0, 0, NULL, 0, NULL, 1, &image_barrier );
    }

    void CommandBuffer::clearAttachments( const core::Framebuffer* framebuffer )
    {
        const vk::Framebuffer* fb = framebuffer->vulkan();

        VkClearRect clear_rect = {};
        clear_rect.baseArrayLayer = 0;
        clear_rect.layerCount = 1;
        clear_rect.rect.offset = {0, 0};
        clear_rect.rect.extent = fb->extent();

        VkClearAttachment clear_attachment_color = {};
        clear_attachment_color.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        clear_attachment_color.colorAttachment = 0; // Color attachment index
        clear_attachment_color.clearValue = fb->clearcolor();
        vkCmdClearAttachments( _handle, 1, &clear_attachment_color, 1, &clear_rect); // Clear color attachment

        if ( fb->hasDepthAttachment() )
        {
            // Use this command to clear the depth-stencil attachment (if you have one):
            VkClearAttachment clear_attachment_depth = {};
            clear_attachment_depth.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            clear_attachment_depth.colorAttachment = VK_ATTACHMENT_UNUSED; // Not a color attachment
            clear_attachment_depth.clearValue = fb->cleardepth();
            vkCmdClearAttachments( _handle, 1, &clear_attachment_depth, 1, &clear_rect); // Clear depth-stencil attachment
        }
    }

    void CommandBuffer::endRenderpass()
    {
        vkCmdEndRenderPass( _handle );
    }

    bool CommandBuffer::begin()
    {
        VkResult result;
//        VkDevice device = vk::Device::get()->logicalDevice();
//
//        VkFence fences[] = { _fence };
//        result = vkWaitForFences( device, 1, fences, VK_TRUE, UINT64_MAX );
//        if (result != VK_SUCCESS)
//        {
//            KEGE_ERROR( vkGetError( result ) );
//            return false;
//        }
//        vkResetFences( device, 1, fences );

        vkResetCommandBuffer( _handle, 0 );
        VkCommandBufferBeginInfo info{};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if ( _primary )
        {
            VkCommandBufferInheritanceInfo inheritance = {};
            inheritance.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
            if ( _primary->_renderpass  )
            {
                inheritance.renderPass = _primary->_renderpass->handle();
                info.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
            }
            if ( _primary->_framebuffer ) 
            {
                inheritance.framebuffer = _primary->_framebuffer->handle();
            }
            inheritance.occlusionQueryEnable = VK_FALSE;

            info.pInheritanceInfo = &inheritance;
            result = vkBeginCommandBuffer( _handle, &info );
        }
        else
        {
            result = vkBeginCommandBuffer( _handle, &info );
        }

        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR( vkGetError( result ) );
            return false;
        }
        _is_recording = true;
        return true;
    }

    bool CommandBuffer::end()
    {
        VkResult result = vkEndCommandBuffer( _handle );
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR( vkGetError( result ) );
            return false;
        }
        _is_recording = false;
        return true;
    }

    void CommandBuffer::bindShader( const vk::Shader* shader )
    {
        const vk::Shader* vs = shader->vulkan();
        vkCmdBindPipeline( _handle, vs->pipelineLayout()->bindPoint(), vs->handle() );
    }

    void CommandBuffer::bindVertexBuffer( const core::Buffer* buffer )
    {
        const VkBuffer buffers[] = { buffer->vulkan()->handle() };
        const VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers( _handle, 0, 1, buffers, offsets );
    }

    void CommandBuffer::bindIndexBuffer( const core::Buffer* buffer, core::Index::Type index_type )
    {
        vkCmdBindIndexBuffer
        (
            _handle,
            buffer->vulkan()->handle(),
            0, vk::Device::get()->getIndexType( index_type )
        );
    }

    void CommandBuffer::pushConstants
    (
        const VkPipelineLayout& pipeline_layout,
        VkShaderStageFlags shader_stages,
        uint32_t offset, uint32_t size,
        const void* data
    )
    {
        vkCmdPushConstants( _handle, pipeline_layout, shader_stages, offset, size, data );
    }

    void CommandBuffer::setViewport( float x, float y, float width, float height )
    {
        VkViewport viewport{};
        viewport.x = x;
        viewport.y = y;
        viewport.width = width;
        viewport.height = height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport( _handle, 0, 1, &viewport );
    }

    void CommandBuffer::setScissor( uint32_t x, uint32_t y, uint32_t width, uint32_t height )
    {
        VkRect2D scissor{};
        scissor.offset.x = x;
        scissor.offset.y = y;
        scissor.extent.width = width;
        scissor.extent.height = height;
        vkCmdSetScissor( _handle, 0, 1, &scissor );
    }

    void CommandBuffer::reset()
    {
        vkResetCommandBuffer( _handle, 0 /*VkCommandBufferResetFlagBits*/ );
    }

    bool CommandBuffer::construct( vk::CommandPool* pool )
    {
        _handle = pool->allocateCommandBuffers( VK_COMMAND_BUFFER_LEVEL_PRIMARY );
        if ( _handle == VK_NULL_HANDLE )
        {
            throw false;
        }
        _command_pool = pool;

        /**
         * If this object is not a secondary command buffer them construct a semaphore object.
         * secondary command-buffers will have a primary command-buffer associated with it.
         * The semaphore object will be use when submiting this command buffer to a queue.
         * @see Renderer::submit() */
        if ( !_primary )
        {
            _signal_semaphore.construct();
            _fence.construct();
        }
        return true;
    }

    void CommandBuffer::destruct()
    {
        if ( _command_pool != VK_NULL_HANDLE )
        {
            _command_pool->freeCommandBuffer( _handle );
            _handle = VK_NULL_HANDLE;
            _command_pool = nullptr;
        }
        destructSyncObjects();
    }


    core::CmdQueueType CommandBuffer::cmdQueueType()const
    {
        return _command_pool->cmdQueueType();
    }

    const VkCommandBuffer CommandBuffer::handle()const
    {
        return _handle;
    }

    VkCommandBuffer CommandBuffer::handle()
    {
        return _handle;
    }

    bool CommandBuffer::isRecording()const
    {
        return true;
    }


    void CommandBuffer::reconstructSyncObjects()
    {
        destructSyncObjects();
        _fence.construct();
        _signal_semaphore.construct();
    }

    void CommandBuffer::destructSyncObjects()
    {
        _signal_semaphore.destruct();
        _fence.destruct();
    }

    vk::Semaphore* CommandBuffer::signalSemaphore()
    {
        return &_signal_semaphore;
    }

    void CommandBuffer::imageTransitionComputeToGraphics( core::Texture* texture )const
    {
        // Transition the image layout back to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        VkImageMemoryBarrier imageBarrier = {};
        imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL; // The previous layout
        imageBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageBarrier.image = texture->vulkan()->image().source().handle(); // The image you're transitioning
        imageBarrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
        imageBarrier.srcAccessMask = 0; // Specify the appropriate access flags
        imageBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier
        (
            _handle,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, // or whichever stage you need next
            0,
            0, nullptr,
            0, nullptr,
            1, &imageBarrier
        );
    }

    void CommandBuffer::pipelineBarrier( core::Texture* texture )const
    {
        VkImageMemoryBarrier image_memory_barrier = {};
        image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        image_memory_barrier.image = texture->vulkan()->image().source().handle(); // The image you're using
        image_memory_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_memory_barrier.newLayout = VK_IMAGE_LAYOUT_GENERAL; // or the layout you need
        image_memory_barrier.srcAccessMask = 0; // Use appropriate access masks
        image_memory_barrier.dstAccessMask = 0; // Use appropriate access masks
        image_memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // Adjust the aspectMask accordingly
        image_memory_barrier.subresourceRange.baseMipLevel = 0;
        image_memory_barrier.subresourceRange.levelCount = 1;
        image_memory_barrier.subresourceRange.baseArrayLayer = 0;
        image_memory_barrier.subresourceRange.layerCount = 1;

        vkCmdPipelineBarrier
        (
            _handle,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            0,
            0, nullptr,
            0, nullptr,
            1, &image_memory_barrier
        );
    }

    vk::Fence* CommandBuffer::fence()
    {
        return &_fence;
    }

    void CommandBuffer::dispatch( uint32_t num_groups_x, uint32_t num_groups_y, uint32_t num_groups_z )
    {
        try 
        {
            vkCmdDispatch( _handle, num_groups_x, num_groups_y, num_groups_z );
        } 
        catch ( const std::runtime_error& err )
        {
            throw err;
        }
        catch ( ... )
        {
            throw std::runtime_error("unknown error CommandBuffer::dispatch");
        }
    }

    CommandBuffer::~CommandBuffer()
    {
        destruct();
    }


    CommandBuffer::CommandBuffer( vk::CommandPool* command_pool, VkCommandBuffer handle, vk::CommandBuffer* primary )
    :   _command_pool( nullptr )
    ,   _handle( handle )
    ,   _is_recording( false )
    ,   _framebuffer( nullptr )
    ,   _renderpass( nullptr )
    ,   _primary( primary )
    {
    }

    CommandBuffer::CommandBuffer( vk::CommandPool* command_pool )
    :   _command_pool( nullptr )
    ,   _handle( VK_NULL_HANDLE )
    ,   _is_recording( false )
    ,   _framebuffer( nullptr )
    ,   _renderpass( nullptr )
    ,   _primary( nullptr )
    {
        if ( _handle == VK_NULL_HANDLE )
        {
            construct( command_pool );
        }
    }

    CommandBuffer::CommandBuffer()
    :   _command_pool( nullptr )
    ,   _handle( VK_NULL_HANDLE )
    ,   _is_recording( false )
    ,   _framebuffer( nullptr )
    ,   _renderpass( nullptr )
    ,   _primary( nullptr )
    {
    }
}}
