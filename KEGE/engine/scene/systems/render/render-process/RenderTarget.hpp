//
//  RenderTarget.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/30/23.
//

#ifndef RenderTarget_hpp
#define RenderTarget_hpp

#include "KEGE.hpp"
#include "Core.hpp"
#include "Graphics.hpp"

namespace kege
{
    class RenderTarget : public kege::RefCounter
    {
    public:

        virtual void beginRenderPass( core::CommandBuffer* command, core::SubpassContents contents ) = 0;
        virtual void clearAttachments( core::CommandBuffer* command ) = 0;
        virtual void endRenderPass( core::CommandBuffer* command ) = 0;
        virtual const core::Framebuffer* getFramebuffer()const = 0;

        virtual core::UniformLayout* getCurrentUniformLayout(){ return nullptr;}

        virtual void swapBuffer() = 0;
        virtual ~RenderTarget(){}
    };
}

#endif /* RenderTarget_hpp */
