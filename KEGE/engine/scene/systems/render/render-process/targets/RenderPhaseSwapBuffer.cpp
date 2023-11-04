//
//  RenderPhaseSwapBuffer.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/24/23.
//

#include "RenderPhaseSwapBuffer.hpp"
namespace kege
{
    class RenderPhaseFBO : public kege::RefCounter
    {
    public:

        RenderPhaseFBO( uint16_t width, uint16_t height );
        kege::Ref< core::Framebuffer > _framebuffer;
        kege::UniformLayout2 _uniform;
    };
    RenderPhaseFBO::RenderPhaseFBO( uint16_t width, uint16_t height )
    {
        core::Device* device = KEGE::core()->graphics()->device();
        kege::Ref< core::Sampler > sampler = device->createSampler
        (
            core::Sampler::LINEAR,
            core::Sampler::LINEAR,
            core::Sampler::CLAMP_TO_EDGE,
            core::Sampler::CLAMP_TO_EDGE,
            core::Sampler::CLAMP_TO_EDGE
        );

        kege::Ref< core::Texture > color_texture = device->createTexture2d
        (
            width, height, 1, core::BGRA8_SRGB, nullptr, false
        );

        kege::Ref< core::Texture > depth_texture = device->createTexture2d
        (
            width, height, 1, core::D32, nullptr, false
        );

        _framebuffer = device->createFramebuffer();
        _framebuffer->attach( core::COLOR_COMPONENT, color_texture );
        _framebuffer->attach( core::DEPTH_COMPONENT, depth_texture );
        if( !_framebuffer->construct() )
        {
            throw std::runtime_error( "" );
        }

        _uniform.setBinding( 0, new kege::TextureSamplerBinding( "ColorAttachment", 0, sampler, color_texture ) );
        _uniform.setBinding( 1, new kege::TextureSamplerBinding( "DepthAttachment", 1, sampler, depth_texture ) );
    }
}









namespace kege
{

    RenderPhaseSwapBuffer::RenderPhaseSwapBuffer(uint16_t width, uint16_t height)
    {
        _render_targets[0] = new kege::RenderPhaseFBO( width, height );
        _render_targets[1] = new kege::RenderPhaseFBO( width, height );
    }

    void RenderPhaseSwapBuffer::beginRenderPass( core::CommandBuffer* command, core::SubpassContents contents )
    {
        const core::Framebuffer* framebuffer = _render_targets[0]->_framebuffer.ref();
        command->beginRenderpass( framebuffer, contents );
    }

    void  RenderPhaseSwapBuffer::clearAttachments( core::CommandBuffer* command )
    {
        const core::Framebuffer* framebuffer = _render_targets[0]->_framebuffer.ref();
        const uint HEIGHT = framebuffer->height();
        const uint WIDTH = framebuffer->width();

        command->setViewport( 0, 0, WIDTH, HEIGHT );
        command->setScissor( 0, 0, WIDTH, HEIGHT );
        command->clearAttachments( framebuffer );
    }

    void RenderPhaseSwapBuffer::endRenderPass( core::CommandBuffer* command )
    {
        command->endRenderpass();
        swapBuffer();
    }

    const core::Framebuffer* RenderPhaseSwapBuffer::getFramebuffer()const
    {
        return _render_targets[0]->_framebuffer.ref();
    }

    core::UniformLayout* RenderPhaseSwapBuffer::getCurrentUniformLayout()
    {
        return &_render_targets[1]->_uniform;
    }

    void RenderPhaseSwapBuffer::swapBuffer()
    {
        kege::Ref< RenderPhaseFBO > fbo = _render_targets[0];
        _render_targets[0] = _render_targets[1];
        _render_targets[1] = fbo;
    }


    RenderPhaseSwapBuffer::~RenderPhaseSwapBuffer()
    {
        _render_targets[0].clear();
        _render_targets[1].clear();
    }
}
