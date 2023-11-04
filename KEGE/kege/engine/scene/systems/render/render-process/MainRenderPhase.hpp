//
//  MainRenderPhase.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/8/23.
//

#ifndef MainRenderPhase_hpp
#define MainRenderPhase_hpp

#include "../render-process/RenderProcess.hpp"

namespace kege
{
    class RenderProcess;

    class MainRenderPhase : public kege::RenderPhase
    {
    public:

        MainRenderPhase( int render_target, int input_target, const std::vector< kege::Ref< RenderTechnique > >& techniques );

        void add( const kege::Ref< RenderTechnique >& technique );
        core::CommandBuffer* getCommandBuffer();
        void execute();
        void begin();

        ~MainRenderPhase();
        
    protected:

        std::vector< kege::Ref< RenderTechnique > > _techniques;
    };
}
#endif /* MainRenderPhase_hpp */
