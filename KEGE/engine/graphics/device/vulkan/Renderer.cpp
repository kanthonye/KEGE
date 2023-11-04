//
//  Renderer.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/9/23.
//

#include <cassert>
#include "Channel.hpp"
#include "Renderer.hpp"
#include "../vulkan/Device.hpp"
#include "../vulkan/CommandBuffer.hpp"
namespace kege{namespace vk{

    VkResult Renderer::waitForFences( std::vector< vk::Fence* > &fences )
    {
        std::vector< VkFence > fence_handles;
        for( auto f : fences) fence_handles.push_back( f->handle() );
        fences.clear();

        uint32_t total_fences = static_cast<uint32_t>( fence_handles.size() );
        VkResult result = vkWaitForFences( vk::Device::get()->logicalDevice(), total_fences, fence_handles.data(), VK_TRUE, UINT64_MAX );
        if (result != VK_SUCCESS)
        {
            KEGE_ERROR( vkGetError( result ) );
            return result;
        }
        vkResetFences( vk::Device::get()->logicalDevice(), total_fences, fence_handles.data() );
        return result;
    }

    bool Renderer::submit( core::CommandBuffer* command )
    {
        vk::CommandBuffer* comm = command->vulkan();

        VkCommandBuffer command_buffers[] = { comm->handle() };

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.pCommandBuffers = command_buffers;
        submit_info.commandBufferCount = 1;
        if ( _frame_started )
        {
            VkSemaphore signal_semaphores[] = { comm->signalSemaphore()->handle() };

            /**
             * At the beginning of every frame the _wait_semaphore is set to the semaphore of
             * this Renderer object. This semaphore does nothing it is just a place holder for
             * submitting the first command buffer. The first submitted command buffer will
             * wait on this semaphore which does nothing, so their is nothing to wait for. Once
             * the `wait_semaphores[] = { _wait_semaphore->handle() }`, we set the member
             * _wait_semaphore to hold the refernce of the command -buffer signal-semaphore
             */
            VkSemaphore wait_semaphores[] = { _wait_semaphore->handle() };

            /**
             * The we set the member _wait_semaphore to hold a refernce of the command-buffer
             * signal-semaphore. while the current frame is executing the next command-buffer
             * to get submitted will wait on the previously submitted command-buffer because
             * of the code above -> `wait_semaphores[] = { _wait_semaphore->handle() }`.
             */
            _wait_semaphore = comm->signalSemaphore();

            submit_info.pSignalSemaphores = signal_semaphores;
            submit_info.signalSemaphoreCount = 1;
            submit_info.pWaitSemaphores = wait_semaphores;
            submit_info.waitSemaphoreCount = 1;

            VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
            submit_info.pWaitDstStageMask = wait_stages;

            /**
             * Collect the command-buffer fence. We need will reset the all when the frame begins again
             */
            _fences[ _current_frame ].push_back( comm->fence() );
        }
        else
        {
            _offscreen_fences[ _offscreen_index ].push_back( comm->fence() );
        }

        VkQueue& graphics_queue = vk::Device::get()->queues( comm->cmdQueueType() );
        VkResult result = vkQueueSubmit( graphics_queue, 1, &submit_info, comm->fence()->handle() );
        if ( result != VK_SUCCESS  )
        {
            KEGE_ERROR( "%s: failed to submit command buffer!", vkGetError( result ) );
            return false;
        }
        return true;
    }

    void Renderer::wait( core::CommandBuffer* command )
    {
        vk::CommandBuffer* comm = command->vulkan();
        const VkFence fences[] = { comm->fence()->handle() };
        vkWaitForFences( vk::Device::get()->logicalDevice(), 1, fences, VK_TRUE, UINT64_MAX );
        vkResetFences( vk::Device::get()->logicalDevice(), 1, fences );
    }

    core::CommandBuffer* Renderer::getCurrentCommandBuffer()
    {
        return &_command_buffers[ _current_frame ];
    }

    VkResult Renderer::waitForOffFrameExecutions()
    {
        VkResult result = VK_SUCCESS;
        if ( !_offscreen_fences[ _offscreen_index ].empty() )
        {
            result = waitForFences( _offscreen_fences[ _offscreen_index ] );
            if (result != VK_SUCCESS)
            {
                return result;
            }
            _offscreen_index = (_offscreen_index + 1) % MAX_FRAMES_IN_FLIGHT;
        }
        return result;
    }

    void Renderer::clearSwapchainFramebuffer()
    {
        getCurrentCommandBuffer()->clearAttachments( _swapchain.frameBuffer( _current_frame ) );
    }

    void Renderer::beginSwapchainRenderpass( core::CommandBuffer* command_buffer, core::SubpassContents contents )
    {
        command_buffer->beginRenderpass( _swapchain.frameBuffer( _current_frame ), contents );
    }

    void Renderer::endSwapchainRenderpass( core::CommandBuffer* command_buffer )
    {
        command_buffer->endRenderpass();
    }

    bool Renderer::beginCommandBuffer()
    {
        return getCurrentCommandBuffer()->begin();
    }

    void Renderer::endCommandBuffer()
    {
        getCurrentCommandBuffer()->end();
        submit( getCurrentCommandBuffer() );
    }

    bool Renderer::beginFrame()
    {
        assert( _frame_started == false && "Renderer::beginFrame() : can not start a frame without ending the previous frame." );

        if ( waitForOffFrameExecutions() != VK_SUCCESS )
        {
            return false;
        }

        _current_frame = (_current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
        if ( !_fences[ _current_frame ].empty() )
        {
            if ( waitForFences( _fences[ _current_frame ] ) != VK_SUCCESS )
            {
                return false;
            }
        }

        VkResult result = vkAcquireNextImageKHR
        (
            vk::Device::get()->logicalDevice(),
            _swapchain.handle(),
            UINT64_MAX,
            _semaphores[ _current_frame ].handle(),
            VK_NULL_HANDLE, &_image_index
         );

        if (result != VK_SUCCESS)
        {
            if ( result == VK_ERROR_OUT_OF_DATE_KHR || _framebuffer_resized )
            {
                std::cout << vkGetError( result )<<": "<< _image_index <<'\n';
                recreateSwapchain();
                return false;
            }
            else if ( result != VK_SUBOPTIMAL_KHR )
            {
                KEGE_ERROR( vkGetError( result ) );
                return false;
            }
        }

        /**
         * update the current frame _wait_semaphore. This will be used for the submission of
         * ths first command-buffer  to this Renderer.
         *
         * @see vk::Renderer::submit()
         */
        _wait_semaphore = &_semaphores[ _current_frame ];
        _frame_started = true;
        return true;
    }

    bool Renderer::endFrame()
    {
        assert( _frame_started == true && "Renderer::endFrame() : can not end a frame that has not been started." );
        _frame_started = false;
        return true;
    }

    void Renderer::receive(const kege::Ref<Message> &message)
    {
        _framebuffer_resized = true;
    }

    const core::Framebuffer* Renderer::getSwapchainFramebuffer()const
    {
        return _swapchain.frameBuffer( _image_index );
    }

    const core::Renderpass* Renderer::getSwapchainRenderpass()const
    {
        return _swapchain.frameBuffer( 0 )->renderpass();
    }

    VkResult Renderer::construct()
    {
        VkResult result = _swapchain.construct();
        if ( result != VK_SUCCESS )
        {
            return result;
        }

        _command_buffers.resize( MAX_FRAMES_IN_FLIGHT );
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            _command_buffers[ i ].construct( vk::Device::get()->commandPool( core::GRAPHICS_QUEUE ) );
            _command_buffers[ i ].reconstructSyncObjects();

            _semaphores[i].construct();
            //_fences[i].construct();
        }
        _wait_semaphore = &_semaphores[ 0 ];

        kege::Channel::add< kege::MsgFramebufferSize, kege::Ref< Message >, Renderer >( &Renderer::receive, this );
        return result;
    }

    uint32_t Renderer::currentFrameIndex()const
    {
        return _current_frame;
    }

    void Renderer::destruct()
    {
        kege::Channel::remove< kege::MsgFramebufferSize, kege::Ref< Message > >( this );
        _command_buffers.clear();
        _swapchain.destruct();

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            //_fences[i].destruct();
            _semaphores[i].destruct();
        }
    }

    bool Renderer::present()
    {
        VkSwapchainKHR swapchains[] = { _swapchain.handle() };
        VkSemaphore wait_semaphores[] = { _wait_semaphore->handle() };

        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.pImageIndices = &_image_index;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = wait_semaphores;
        present_info.swapchainCount = 1;
        present_info.pSwapchains = swapchains;

        VkQueue present_queue = vk::Device::get()->queues( core::GRAPHICS_QUEUE );
        VkResult result = vkQueuePresentKHR( present_queue, &present_info );
        if ( result != VK_SUCCESS )
        {
            if ( result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _framebuffer_resized )
            {
                std::cout << vkGetError( result )<<", "<< _image_index <<'\n';
                recreateSwapchain();
            }
            else
            {
                KEGE_ERROR( vkGetError( result ) );
                return false;
            }
        }
        return true;
    }

    void Renderer::recreateSwapchain()
    {
        int width = 0, height = 0;
        glfwGetFramebufferSize( vk::Device::get()->window()->vulkan()->handle(), &width, &height );
        while (width == 0 || height == 0)
        {
            glfwGetFramebufferSize( vk::Device::get()->window()->vulkan()->handle(), &width, &height );
            glfwWaitEvents();
        }
        vk::Device::get()->waitIdle();

        _framebuffer_resized = false;
        _swapchain.destruct();
        _swapchain.construct();

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            _semaphores[i].destruct();
            _semaphores[i].construct();
        }
    }

    Renderer::~Renderer()
    {
        destruct();
    }

    Renderer::Renderer()
    :   _framebuffer_resized( false )
    ,   _current_frame( -1 )
    ,   _image_index( 0 )
    ,   _frame_started( false )
    ,   _wait_semaphore( nullptr )
    ,   _offscreen_index( 0 )
    {
    }

}}
