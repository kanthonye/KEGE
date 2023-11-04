//
//  RenderPhaseSwapBuffer.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/24/23.
//

#ifndef RenderPhaseSwapBuffer_hpp
#define RenderPhaseSwapBuffer_hpp

#include "RenderTarget.hpp"

namespace kege
{
    class RenderPhaseFBO;
    
    class RenderPhaseSwapBuffer : public kege::RenderTarget
    {
    public:

        RenderPhaseSwapBuffer( uint16_t width, uint16_t height );
        void beginRenderPass( core::CommandBuffer* command, core::SubpassContents contents );

        void clearAttachments( core::CommandBuffer* command );
        void endRenderPass( core::CommandBuffer* command );
        const core::Framebuffer* getFramebuffer()const;
        core::UniformLayout* getCurrentUniformLayout();
        void swapBuffer();

        ~RenderPhaseSwapBuffer();

    private:

        kege::Ref< RenderPhaseFBO > _render_targets[2];
    };
}
#endif /* RenderPhaseSwapBuffer_hpp */
