//
//  CommandBuffer.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/2/23.
//

#ifndef KEGE_VULKAN_COMMANDBUFFER
#define KEGE_VULKAN_COMMANDBUFFER
#include "../../device/core/CommandBuffer.hpp"
#include "../../device/vulkan/utils.hpp"
namespace kege{namespace vk{
    class Shader;
    class Device;

    /**
     * @brief CommandBuffer is a concrete implementation of the core::CommandBuffer interface
     * for Vulkan-based command buffer management.
     *
     * The CommandBuffer class provides functionality for recording and managing command buffers
     * used in rendering and compute operations with Vulkan. It offers methods for drawing, binding
     * resources, managing render passes, and more.
     *
     * @see core::CommandBuffer, core::UniformAsset, vk::Shader
     */
    class CommandBuffer : public core::CommandBuffer
    {
    public:

        /**
         * @brief Execute secondary command buffer associated with this parent command buffer
         * @param command_buffers A vector of command buffers to execute
         */
        void executeCommands( const std::vector< core::CommandBuffer* >& command_buffers );

        /**
         * @brief Push constant values to the shaders.
         * @param pipeline_layout The pipeline layout associated with the shader.
         * @param shader_stages The shader stages affected by the push constants.
         * @param offset The offset at which to push the constants.
         * @param size The size of the push constants data.
         * @param data A pointer to the push constants data.
         */
        void pushConstants
        (   const VkPipelineLayout& pipeline_layout, VkShaderStageFlags shader_stages
         ,  uint32_t offset
         ,  uint32_t size
         ,  const void* data  );

        /**
         * @brief Draw indexed primitives.
         * @param index_count The number of indices to draw.
         * @param instance_count The number of instances to draw.
         * @param first_index The starting index.
         * @param vertex_offset The offset of the first vertex.
         * @param first_instance The first instance.
         */
        void drawIndexed( uint32_t index_count, uint32_t instance_count, uint32_t first_index, int32_t vertex_offset, uint32_t first_instance );

        /**
         * @brief Draw non-indexed primitives.
         * @param vertex_count The number of vertices to draw.
         * @param instance_count The number of instances to draw.
         * @param first_vertex The starting vertex.
         * @param first_instance The first instance.
         */
        void draw( uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance );

        /**
         * @brief Bind a UniformAsset to a specific Shader.
         * @param first_set The set index to bind to.
         * @param layout A pointer to the UniformAsset to be bound.
         * @param shader A pointer to the Shader to which the asset should be bound.
         */
        void bindUniformLayout( uint32_t first_set, const core::UniformLayout* layout, vk::Shader* shader );

        /**
         * @brief Bind a UniformAsset to all graphics pipelines that need resources linked to the set index.
         * @param first_set The first set to bind.
         * @param layout A pointer to the UniformAsset to be bound.
         */
        void bindUniformLayout( uint32_t first_set, const core::UniformLayout* layout );

        /**
         * @brief Bind an index buffer to the command buffer for indexed rendering.
         * @param buffer A pointer to the index buffer.
         * @param index_type The type of indices in the buffer.
         */
        void bindIndexBuffer( const core::Buffer* buffer, core::Index::Type index_type );

        /**
         * @brief Bind vertex buffers to the command buffer.
         * @param buffer A pointer to the vertex buffer.
         */
        void bindVertexBuffer( const core::Buffer* buffer );

        /**
         * @brief Bind a graphics or compute shader to the command buffer.
         * @param shader A pointer to the shader to bind.
         */
        void bindShader( const vk::Shader* shader );

        void setImageLayout
        (
         VkImage image,
         VkImageAspectFlags aspects,
         VkImageLayout old_layout,
         VkImageLayout new_layout
        );

        /**
         * @brief Set the scissor rectangle for rendering.
         * @param x The x-coordinate of the scissor rectangle.
         * @param y The y-coordinate of the scissor rectangle.
         * @param width The width of the scissor rectangle.
         * @param height The height of the scissor rectangle.
         */
        void setScissor( uint32_t x, uint32_t y, uint32_t width, uint32_t height );

        /**
         * @brief Set the viewport parameters for rendering.
         * @param x The x-coordinate of the viewport.
         * @param y The y-coordinate of the viewport.
         * @param width The width of the viewport.
         * @param height The height of the viewport.
         */
        void setViewport( float x, float y, float width, float height );

        kege::Ref< core::CommandBuffer > createSecondaryCommandBuffer();

        /**
         * @brief Begin a render pass with the given framebuffer.
         * @param framebuffer A pointer to the framebuffer to use in the render pass.
         * @param contents: Specifies how the commands within the subpass will be provided.
         */
        void beginRenderpass( const core::Framebuffer* framebuffer, core::SubpassContents contents );

        /**
         * @brief Clear specified attachments within the active render pass.
         */
        void clearAttachments( const core::Framebuffer* framebuffer );

        /**
         * @brief End the current render pass.
         */
        void endRenderpass();

        /**
         * @brief Dispatch a compute shader with specified workgroups.
         */
        void dispatch( uint32_t num_groups_x, uint32_t num_groups_y, uint32_t num_groups_z );

        /**
         * @brief Reset the command buffer.
         */
        void reset();

        /**
         * @brief Start recording the command buffer.
         * @return true if recording was successfully started; false otherwise.
         */
        bool begin();

        /**
         * @brief End recording the command buffer.
         * @return true if recording was successfully ended; false otherwise.
         */
        bool end();

        /**
         * @brief Construct the command buffer.
         * @param pool A pointer to the command pool to allocate the command buffer.
         * @return true if construction was successful; false otherwise.
         */
        bool construct( vk::CommandPool* pool );

        /**
         * @brief Destruct the command buffer.
         */
        void destruct();

        /**
         * @brief Get the Vulkan handle for the command buffer.
         * @return A pointer to the Vulkan command buffer.
         */
        const vk::CommandBuffer* vulkan()const{return this;}

        /**
         * @brief Get the Vulkan handle for the command buffer.
         * @return A pointer to the Vulkan command buffer.
         */
        vk::CommandBuffer* vulkan(){return this;}


        core::CmdQueueType cmdQueueType()const;
        
        /**
         * @brief Get the handle of the Vulkan command buffer.
         * @return The Vulkan command buffer handle.
         */
        const VkCommandBuffer handle()const;

        /**
         * @brief Get the handle of the Vulkan command buffer.
         * @return The Vulkan command buffer handle.
         */
        VkCommandBuffer handle();


        void reconstructSyncObjects();
        void destructSyncObjects();

        /**
         * @brief TODO
         */
        void imageTransitionComputeToGraphics( core::Texture* texture )const;
        void pipelineBarrier( core::Texture* texture )const;

        /**
         * @brief Get the signal semaphore associated with the command buffer.
         * @return The Vulkan semaphore used for signaling.
         */
        vk::Semaphore* signalSemaphore();

        /**
         * @brief Get the wait semaphore associated with the command buffer.
         * @return The Vulkan semaphore used for waiting.
         */
        //vk::Semaphore* waitSemaphore();

        /**
         * @brief Get the fence associated with the command buffer.
         * @return The Vulkan fence used for synchronization.
         */
        vk::Fence* fence();

        /**
         * @brief Check if the command buffer recording is in progress.
         * @return true if the command buffer is in recording mode; false otherwise.
         */
        bool isRecording()const;

        /**
         * @brief Constructor for CommandBuffer.
         * @param command_pool A pointer to the command pool to allocate with this command buffer.
         * @param handle the Vulkan command buffer handle for this CommandBuffer object
         * @param primary The primary command buffer this CommandBuffer object inherits from
         */
        CommandBuffer( vk::CommandPool* command_pool, VkCommandBuffer handle, vk::CommandBuffer* primary );

        /**
         * @brief Constructor for CommandBuffer.
         * @param command_pool A pointer to the command pool to allocate with this command buffer.
         */
        CommandBuffer( vk::CommandPool* command_pool );

        /**
         * @brief Destructor for CommandBuffer.
         */
        ~CommandBuffer();

        /**
         * @brief Constructor for CommandBuffer.
         */
        CommandBuffer();

    private:

        const vk::Framebuffer* _framebuffer;
        const vk::Renderpass* _renderpass;

        vk::CommandBuffer* _primary;

        /**
         * The command pool to which the command buffer belongs.
         */
        vk::CommandPool* _command_pool;

        /**
         * The handle of the Vulkan command buffer.
         */
        VkCommandBuffer _handle;

        /**
         * The signal semaphore for signaling.
         */
        vk::Semaphore _signal_semaphore;

        /**
         * The wait semaphore for waiting.
         */
        //vk::Semaphore _wait_semaphore;

        /**
         * The fence for synchronization.
         */
        vk::Fence _fence;

        /**
         * Flag indicating if recording is in progress.
         */
        bool _is_recording;
    };

}}
#endif /* KEGE_VULKAN_COMMANDBUFFER */
