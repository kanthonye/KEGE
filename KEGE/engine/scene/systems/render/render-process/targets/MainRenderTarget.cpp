//
//  MainRenderTarget.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/30/23.
//

#include "MainRenderTarget.hpp"
namespace kege
{

    void MainRenderTarget::beginRenderPass( core::CommandBuffer* command, core::SubpassContents contents )
    {
        KEGE::core()->graphics()->renderer()->beginSwapchainRenderpass( command, contents );
    }

    void  MainRenderTarget::clearAttachments( core::CommandBuffer* command )
    {
        const core::Framebuffer* framebuffer = KEGE::core()->graphics()->renderer()->getSwapchainFramebuffer();
        const uint HEIGHT = framebuffer->height();
        const uint WIDTH = framebuffer->width();

        command->setViewport( 0, 0, WIDTH, HEIGHT );
        command->setScissor( 0, 0, WIDTH, HEIGHT );
        command->clearAttachments( framebuffer );
    }

    void MainRenderTarget::endRenderPass( core::CommandBuffer* command )
    {
        KEGE::core()->graphics()->renderer()->endSwapchainRenderpass( command );
    }

    const core::Framebuffer* MainRenderTarget::getFramebuffer()const
    {
        return KEGE::core()->graphics()->renderer()->getSwapchainFramebuffer();
    }

    void MainRenderTarget::swapBuffer()
    {}

    MainRenderTarget::MainRenderTarget()
    {}
}
