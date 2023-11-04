//
//  RenderPhaseMCB.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/8/23.
//

#ifndef RenderPhaseMCB_hpp
#define RenderPhaseMCB_hpp

#include "../render-process/RenderPhase.hpp"

namespace kege
{
    struct RenderPhaseMCBElement
    {
    public:

        RenderPhaseMCBElement( kege::Ref< core::CommandBuffer > commands[2], kege::Ref< kege::RenderTechnique > technique );
        core::CommandBuffer* getCommandBuffer( uint32_t frame_index );
        kege::RenderTechnique* getRenderTechnique();
        ~RenderPhaseMCBElement();

    private:
        
        kege::Ref< core::CommandBuffer > _command_buffers[2];
        kege::Ref< kege::RenderTechnique > _technique;
    };


    /**
     * @brief RenderPhaseMCB (Render Phase Multi Command Buffer) is a specialized implementation
     * of RenderPhase. A RenderPhase refer to a specific stage or step in the rendering process.
     *
     * RenderPhaseMCB is a concrete subclass of RenderPhase. It manages kege::RenderTechniques
     * and their assocaited kege::core::CommandBuffers.
     *
     * @see kege::RenderProcess, kege::RenderTarget, core::CommandBuffers
     */
    class RenderPhaseMCB : public kege::RenderPhase
    {
    public:

        RenderPhaseMCB( int render_target, int input_target, const std::vector< kege::Ref< RenderTechnique > >& techniques );

        virtual void setRenderTechniques( const std::vector< kege::Ref< RenderTechnique > >& techniques );
        virtual void addRenderTechniques( const kege::Ref< RenderTechnique >& technique );

        virtual core::CommandBuffer* getCommandBuffer();
        virtual void execute();
        
        virtual ~RenderPhaseMCB();

    protected:

        std::vector< RenderPhaseMCBElement > _elements;
        kege::Ref< core::CommandBuffer > _commands[2];
        kege::Ref< core::CommandBuffer > _clear_comm_bufrs[2];
    };
}
#endif /* RenderPhaseMCB_hpp */
