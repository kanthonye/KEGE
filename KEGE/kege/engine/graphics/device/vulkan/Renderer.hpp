//
//  Renderer.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/9/23.
//

#ifndef KEGE_VULKAN_RENDERER_HPP
#define KEGE_VULKAN_RENDERER_HPP
#include "../../device/core/Device.hpp"
#include "../../device/vulkan/utils.hpp"
#include "../../device/vulkan/Swapchain.hpp"
#include "../../device/vulkan/CommandBuffer.hpp"
namespace kege{namespace vk{

    /**
     * @brief Renderer is a concrete implementation of the core::Renderer interface
     * for Vulkan-based rendering.
     *
     * The Renderer class provides functionality for managing the rendering process
     * using the Vulkan graphics API. It handles rendering commands, synchronization,
     * and presentation to the screen.
     *
     * @see core::Renderer, vk::CommandBuffer, vk::Swapchain, core::Renderpass
     */
    class Renderer : public core::Renderer
    {
    public:

        static VkResult waitForFences( std::vector< vk::Fence* > &fences );

        /**
         * @brief Receive a message for framebuffer resize.
         * @param message A reference to the incoming message.
         */
        void receive( const kege::Ref< Message >& message );

        /**
         * @brief Submit a command buffer to a rendering queue.
         * @param command A pointer to the command buffer to be submitted.
         * @return true if the submission was successful; false otherwise.
         */
        bool submit( core::CommandBuffer* command );

        /**
         * @brief Wait for the command buffer to finish execution
         * @param command A pointer to the command buffer to wait for.
         */
        void wait( core::CommandBuffer* command );

        /**
         * @brief Get the main framebuffer associated with the swapchain.
         * @return A pointer to the main framebuffer for the swapchain.
         */
        const core::Framebuffer* getSwapchainFramebuffer()const;

        /**
         * @brief Get the main renderpass associated with the swapchain.
         * @return A pointer to the main renderpass for the swapchain.
         */
        const core::Renderpass* getSwapchainRenderpass()const;

        /**
         * @brief Get the current main command buffer for rendering.
         * @return A pointer to the current main command buffer.
         */
        core::CommandBuffer* getCurrentCommandBuffer();

        /**
         * @brief End compute execution
         */
        VkResult waitForOffFrameExecutions();

        /**
         * @brief Clear the swapchain framebuffer.
         */
        void clearSwapchainFramebuffer();

        /**
         * @brief Begin the rendering pass for the swapchain.
         */
        void beginSwapchainRenderpass( core::CommandBuffer* command_buffer, core::SubpassContents contents );

        /**
         * @brief End the rendering pass for the swapchain.
         */
        void endSwapchainRenderpass( core::CommandBuffer* command_buffer );

        /**
         * @brief Begin recording the main command buffer.
         * @return true if the recording was successfully started; false otherwise.
         */
        bool beginCommandBuffer();

        /**
         * @brief End recording the main command buffer.
         */
        void endCommandBuffer();

        /**
         * @brief Begin the frame rendering process.
         * @return true if the frame rendering process was successfully started; false otherwise.
         */
        bool beginFrame();

        /**
         * @brief End the frame rendering process.
         * @return true if rendering for the frame is completed; false otherwise.
         */
        bool endFrame();

        /**
         * @brief Get the index of the current frame.
         * @return The index of the current frame.
         */
        uint32_t currentFrameIndex()const;

        /**
         * @brief Construct the Renderer object.
         * @return The result of the construction (VkResult).
         */
        VkResult construct();

        /**
         * @brief Destruct the Renderer object.
         */
        void destruct();

        /**
         * @brief Present the rendered image to the screen.
         */
        bool present();

        /**
         * @brief Destructor for the Renderer object.
         */
        ~Renderer();

        /**
         * @brief Default constructor for the Renderer object.
         */
        Renderer();

    private:

        /**
         * @brief Recreate the swapchain when necessary.
         */
        void recreateSwapchain();

    private:

        /**
         * An array of command buffers for rendering (one for each frame).
         */
        std::vector< vk::CommandBuffer > _command_buffers;

        vk::Semaphore _semaphores[ MAX_FRAMES_IN_FLIGHT ];
        vk::Semaphore* _wait_semaphore;

        /**
         * fences that were submitted while no frame was recording, thats must be reset,
         */
        std::vector< vk::Fence* > _offscreen_fences[ MAX_FRAMES_IN_FLIGHT ];
        int32_t _offscreen_index;

        /**
         * fences that were submitted durring a recording frame thats must be reset
         */
        std::vector< vk::Fence* > _fences[ MAX_FRAMES_IN_FLIGHT ];

        /**
         * The swapchain associated with the Renderer.
         */
        vk::Swapchain _swapchain;

        /**
         * Flag indicating whether the framebuffer was resized.
         */
        bool _framebuffer_resized;

        /**
         * The index of the current frame.
         */
        int32_t _current_frame;

        /**
         * The index of the image being rendered.
         */
        uint32_t _image_index;

        /**
         * Flag indicating whether a frame rendering has started.
         */
        bool _frame_started;
    };
}}
#endif /* KEGE_VULKAN_RENDERER_HPP */
