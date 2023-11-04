//
//  RenderingProcess.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/24/23.
//

#ifndef RenderingProcess_hpp
#define RenderingProcess_hpp

#include "../render-process/RenderPhase.hpp"
#include "../render-process/RenderTarget.hpp"
#include "../render-process/RenderTechnique.hpp"

namespace kege
{
    class PhaseBuffer;
    class GeometryBuffer;

    class RenderProcess : public kege::RefCounter
    {
    public:

        void setRenderTargets( const std::map< uint32_t, kege::Ref< RenderTarget > >& targets );
        void addRenderTarget( uint32_t index, const kege::Ref< RenderTarget >& target );

        void setRenderPhases( const std::vector< kege::Ref< RenderPhase > >& phases );
        void addRenderPhase( kege::Ref< RenderPhase > phase );

        RenderTarget* getRenderTarget( uint32_t i );
        int frameIndex()const;

        void execute();
        void present();

        ~RenderProcess();
        RenderProcess();

    private:

        void executeRenderPhase( RenderPhase* phase );

    private:

        std::map< uint32_t, kege::Ref< RenderTarget > > _render_targets;
        std::vector< kege::Ref< RenderPhase > > _phases;
        int _frame_index;
    };
}

#endif /* RenderingProcess_hpp */
