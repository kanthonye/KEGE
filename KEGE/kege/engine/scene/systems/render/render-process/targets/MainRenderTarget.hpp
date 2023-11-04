//
//  MainRenderTarget.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/30/23.
//

#ifndef MainRenderTarget_hpp
#define MainRenderTarget_hpp

#include "RenderTarget.hpp"

namespace kege
{

    class MainRenderTarget : public kege::RenderTarget
    {
    public:

        void beginRenderPass( core::CommandBuffer* command, core::SubpassContents contents );
        void clearAttachments( core::CommandBuffer* command );
        void endRenderPass( core::CommandBuffer* command );
        const core::Framebuffer* getFramebuffer()const;
        void swapBuffer();

        MainRenderTarget( );
    };
}
#endif /* MainRenderTarget_hpp */
