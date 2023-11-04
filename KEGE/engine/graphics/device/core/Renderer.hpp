//
//  Renderer.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/16/23.
//

#ifndef KEGE_CORE_RENDERER_HPP
#define KEGE_CORE_RENDERER_HPP

#include "../core/RenderPass.hpp"
#include "../core/CommandBuffer.hpp"

namespace kege{namespace core{
    
    /**
     * @brief Renderer is an abstract base class for managing rendering operations.
     *
     * The Renderer class defines a common interface for rendering operations. It is
     * designed to be subclassed by concrete rendering implementations, such as Vulkan
     * or DirectX renderers, to provide platform-specific rendering functionality.
     *
     * @see core::Renderpass, core::CommandBuffer
     */
    class Renderer : public kege::RefCounter
    {
    public:

        /**
         * @brief Get the main framebuffer associated with the swapchain.
         * @return A pointer to the main framebuffer for the swapchain.
         */
        virtual const core::Framebuffer* getSwapchainFramebuffer()const = 0;

        /**
         * @brief Get the main renderpass associated with the swapchain.
         * @return A pointer to the main renderpass for the swapchain.
         */
        virtual const core::Renderpass* getSwapchainRenderpass()const = 0;

        /**
         * @brief Get the current main command buffer for rendering.
         * @return A pointer to the current main command buffer.
         */
        virtual core::CommandBuffer* getCurrentCommandBuffer()=0;

        /**
         * @brief Submit a command buffer to a rendering queue.
         * @param command A pointer to the command buffer to be submitted.
         * @return true if the submission was successful; false otherwise.
         */
        virtual bool submit( core::CommandBuffer* command ) = 0;

        /**
         * @brief Wait for the command buffer to finish execution
         * @param command A pointer to the command buffer to wait for.
         */
        virtual void wait( core::CommandBuffer* command ) = 0;

        /**
         * @brief Get the index of the current frame.
         * @return The index of the current frame.
         */
        virtual uint32_t currentFrameIndex()const = 0;

        /**
         * @brief Clear swapchain framebuffer attachments.
         */
        virtual void clearSwapchainFramebuffer() = 0;

        /**
         * @brief Begin the rendering pass for the swapchain.
         * @param command_buffer The command buffer to begine for the render pass with.
         */
        virtual void beginSwapchainRenderpass( core::CommandBuffer* command_buffer, core::SubpassContents contents ) = 0;

        /**
         * @brief End the rendering pass for the swapchain.
         * @param command_buffer The command buffer to end the render pass for.
         */
        virtual void endSwapchainRenderpass( core::CommandBuffer* command_buffer ) = 0;

        /**
         * @brief Begin recording the main command buffer.
         * @return true if the recording was successfully started; false otherwise.
         */
        virtual bool beginCommandBuffer()=0;

        /**
         * @brief End recording the main command buffer.
         */
        virtual void endCommandBuffer()=0;

        /**
         * @brief Begin the frame rendering process.
         * @return true if the frame rendering process was successfully started; false otherwise.
         */
        virtual bool beginFrame()=0;

        /**
         * @brief End the frame rendering process.
         * @return true if rendering for the frame is completed; false otherwise.
         */
        virtual bool endFrame()=0;

        /**
         * @brief Present the rendered image to the screen.
         */
        virtual bool present() = 0;

        KEGE_CORE_ABSTRACTION( Renderer );
    };

}}
#endif /* KEGE_CORE_RENDERER_HPP */
