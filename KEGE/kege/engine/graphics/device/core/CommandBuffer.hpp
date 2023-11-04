//
//  CommandBuffer.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/2/23.
//

#ifndef KEGE_CORE_COMMANDBUFFER
#define KEGE_CORE_COMMANDBUFFER
#include "../../device/core/Framebuffer.hpp"
#include "../../device/core/Buffer.hpp"
#include "../../device/core/Texture.hpp"
#include "../../device/core/UniformLayout.hpp"
namespace kege{namespace vk{
    class CommandBuffer;
}}

namespace kege{namespace core{
    class ClearAttachment;

    typedef struct Viewport {
        float    x;
        float    y;
        float    width;
        float    height;
        float    minDepth;
        float    maxDepth;
    } Viewport;

    typedef struct Scissor {
        uint32_t    x;
        uint32_t    y;
        uint32_t    width;
        uint32_t    height;
    } Scissor;


    class ShaderStageFlags {
    };

    enum CmdQueueType
    {
        GRAPHICS_QUEUE,
        COMPUTE_QUEUE,
        TRANSFER_QUEUE,
        PRESENT_QUEUE,
        MAX_QUEUE_COUNT
    };

    typedef enum CmdUsage
    {
        COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT      = 0b00000001,
        COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT = 0b00000010,
        COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT     = 0b00000100
    } CmdUsage;


    /**
     * @brief Specifies how the commands within the subpass will be provided. 
     * It can take one of two values: 
     *
     * @b VK_SUBPASS_CONTENTS_INLINE: The commands
     * for the subpass will be recorded inline within the primary command buffer.
     *
     * @b VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS: The commands for the subpass
     * will be recorded in secondary command buffers that are executed within the
     * primary command buffer.
     */
    typedef enum SubpassContents
    {
        /**
         * Specify that the commands for the subpass will be recorded inline within the
         * primary command buffer.
         */
        SUBPASS_CONTENTS_PRIMARY_COMMAND_BUFFERS,

        /**
         * Specify that the commands for the subpass will be recorded in secondary command
         * buffers that are executed within the primary command buffer.
         */
        SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS
    } SubpassContents;

    class CommandBuffer : public kege::RefCounter
    {
    public:

        virtual void executeCommands( const std::vector< core::CommandBuffer* >& command_buffers ) = 0;
        virtual kege::Ref< core::CommandBuffer > createSecondaryCommandBuffer(){return nullptr;}

        /**
         * @brief Draw indexed primitives.
         * @param index_count The number of indices to draw.
         * @param instance_count The number of instances to draw.
         * @param first_index The starting index.
         * @param vertex_offset The offset of the first vertex.
         * @param first_instance The first instance.
         */
        virtual void drawIndexed
        (   uint32_t index_count
         ,  uint32_t instance_count
         ,  uint32_t first_index
         ,  int32_t vertex_offset
         ,  uint32_t first_instance  ) = 0;

        /**
         * @brief Draw non-indexed primitives.
         * @param vertex_count The number of vertices to draw.
         * @param instance_count The number of instances to draw.
         * @param first_vertex The starting vertex.
         * @param first_instance The first instance.
         */
        virtual void draw
        (   uint32_t vertex_count
         ,  uint32_t instance_count
         ,  uint32_t first_vertex
         ,  uint32_t first_instance  ) = 0;

        /**
         * @brief Bind a UniformAsset to all graphics pipelines that need resources linked to the set index.
         * @param first_set The first set to bind.
         * @param layout A pointer to the UniformLayout to be bound.
         */
        virtual void bindUniformLayout
        (
            uint32_t first_set,
            const core::UniformLayout* layout
        ) = 0;

        /**
         * @brief Bind an index buffer to the command buffer for indexed rendering.
         * @param buffer A pointer to the index buffer.
         * @param index_type The type of indices in the buffer.
         */
        virtual void bindIndexBuffer( const core::Buffer* buffer, core::Index::Type index_type ) = 0;

        /**
         * @brief Bind vertex buffers to the command buffer.
         * @param buffer A pointer to the vertex buffer.
         */
        virtual void bindVertexBuffer( const core::Buffer* buffer ) = 0;

        /**
         * @brief Begin a render pass with the given framebuffer.
         * @param framebuffer A pointer to the framebuffer to use in the render pass.
         * @param contents Specifies how the commands within the subpass will be provided.
         */
        virtual void beginRenderpass( const core::Framebuffer* framebuffer, core::SubpassContents contents ) = 0;

        /**
         * @brief Clear specified attachments within the active render pass.
         */
        virtual void clearAttachments( const core::Framebuffer* framebuffer ) = 0;

        /**
         * @brief Set the scissor rectangle for rendering.
         * @param x The x-coordinate of the scissor rectangle.
         * @param y The y-coordinate of the scissor rectangle.
         * @param width The width of the scissor rectangle.
         * @param height The height of the scissor rectangle.
         */
        virtual void setScissor
        (   uint32_t x
         ,  uint32_t y
         ,  uint32_t width
         ,  uint32_t height
        ) = 0;

        /**
         * @brief Set the viewport parameters for rendering.
         * @param x The x-coordinate of the viewport.
         * @param y The y-coordinate of the viewport.
         * @param width The width of the viewport.
         * @param height The height of the viewport.
         */
        virtual void setViewport
        (   float x
         ,  float y
         ,  float width
         ,  float height
        ) = 0;

        virtual void imageTransitionComputeToGraphics( core::Texture* texture )const = 0;
        virtual void pipelineBarrier( core::Texture* texture )const = 0;

        /**
         * @brief End the current render pass.
         */
        virtual void endRenderpass() = 0;

        /**
         * @brief Check if the command buffer recording is in progress.
         * @return true if the command buffer is in recording mode; false otherwise.
         */
        virtual bool isRecording()const = 0;

        /**
         * @brief Reset the command buffer.
         */
        virtual void reset() = 0;

        /**
         * @brief Start recording the command buffer.
         * @return true if recording was successfully started; false otherwise.
         */
        virtual bool begin() = 0;

        /**
         * @brief End recording the command buffer.
         * @return true if recording was successfully ended; false otherwise.
         */
        virtual bool end() = 0;

        virtual const vk::CommandBuffer* vulkan()const{return nullptr;}
        virtual vk::CommandBuffer* vulkan(){return nullptr;}

        virtual void destruct() = 0;

        KEGE_CORE_ABSTRACTION( CommandBuffer );
    };
}}
#endif /* KEGE_CORE_COMMANDBUFFER */
