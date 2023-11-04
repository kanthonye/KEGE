//
//  RenderPhase.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/24/23.
//

#ifndef RenderPhase_hpp
#define RenderPhase_hpp

#include "../render-process/RenderTarget.hpp"
#include "../render-process/RenderTechnique.hpp"

namespace kege
{
    class RenderProcess;

    /**
     * @brief RenderPhase refer to a specific stage or step in the rendering process.
     *
     * RenderPhase is responsible for starting the recording process for command buffers,
     * and begin the RenderPass that is associated with the RenderPhase.
     *
     * @see kege::RenderProcess, kege::RenderTarget, core::CommandBuffers
     */
    class RenderPhase : public kege::RefCounter
    {
    public:

        RenderPhase( int render_target, int input_target );

        virtual core::CommandBuffer* getCommandBuffer() = 0;
        kege::RenderTarget* getCurrentRenderTarget();
        kege::RenderTarget* getInputRenderTarget();

        virtual void execute() = 0;
        virtual void begin();
        virtual void end();

        void setRenderTargetIndex( int index );
        virtual ~RenderPhase();

    protected:
        
        core::SubpassContents _subpass_contents;
        kege::RenderTarget* _render_target;
        kege::RenderTarget* _input_target;
        kege::RenderProcess* _process;
        friend kege::RenderProcess;
        
        int _current_target_index;
        int _input_target_index;
    };
}
#endif /* RenderPhase_hpp */
