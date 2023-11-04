//
//  Device.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/11/23.
//

#ifndef KEGE_CORE_DEVICE
#define KEGE_CORE_DEVICE
#include <stdlib.h>
#include "stb_image.h"
#include "../../../memory/pool-system/PoolSysm.hpp"
#include "../../device/core/Buffer.hpp"
#include "../../device/core/Window.hpp"
#include "../../device/core/Texture.hpp"
#include "../../device/core/Sampler.hpp"
#include "../../device/core/Renderer.hpp"
#include "../../device/core/Shader.hpp"
#include "../../device/core/UniformLayout.hpp"
namespace kege{
    class Graphics;
}

namespace kege{namespace core{
    class Window;
    class CommandBuffer;
    class Renderer;
    class RenderPass;



    class Device
    {
    public:

        virtual kege::Ref< core::Shader > createGraphicsShader( const core::Shader::Info& info, const core::Renderpass* renderpass ) = 0;
        virtual kege::Ref< core::Shader > createComputeShader( const core::Shader::Info& info ) = 0;
        virtual kege::Ref< core::CommandBuffer > createCommandBuffer( CmdQueueType quetype ) = 0;
        virtual kege::Ref< core::Framebuffer > createFramebuffer() = 0;

        virtual kege::Ref< core::Texture > createTexture2d
        (
            uint32_t width,
            uint32_t height,
            uint32_t mip_levels,
            core::ImgFormat format,
            const void* data,
            bool storage
        ) = 0;

        virtual kege::Ref< core::Sampler > createSampler
        (
            core::Sampler::Filter min,
            core::Sampler::Filter mag,
            core::Sampler::Wrapping x,
            core::Sampler::Wrapping y,
            core::Sampler::Wrapping z
        ) = 0;

        virtual kege::Ref< core::Buffer > createBuffer
        (
            core::Buffer::Type type,
            uint64_t size,
            const void* data,
            core::Buffer::Usage usage
        ) = 0;

        virtual core::Renderer* renderer() = 0;
        virtual core::Window* window() = 0;
        virtual void waitIdle() = 0;

        virtual void destroyWindow( core::Window* window ) = 0;
        virtual void destruct() = 0;

    protected:

        core::Device& operator=( const core::Device& device ) = delete;
        virtual ~Device(){}
        Device() = default;
        friend kege::Graphics;
    };
}}
#endif /* KEGE_CORE_DEVICE */
