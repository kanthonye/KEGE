//
//  RenderingProcess.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/24/23.
//

#include "../render-process/RenderProcess.hpp"

namespace kege
{
    void RenderProcess::setRenderTargets( const std::map< uint32_t, kege::Ref< RenderTarget > >& targets )
    {
        _render_targets = targets;
    }

    void RenderProcess::addRenderTarget( uint32_t index, const kege::Ref< RenderTarget >& target )
    {
        _render_targets[ index ] = target;
    }

    void RenderProcess::setRenderPhases( const std::vector< kege::Ref< RenderPhase > >& phases )
    {
        _phases = phases;
        for(kege::Ref< RenderPhase >& phase : _phases )
        {
            phase->_process = this;
        }
    }

    void RenderProcess::addRenderPhase( kege::Ref< RenderPhase > phase )
    {
        _phases.push_back( phase );
        phase->_process = this;
    }

    kege::RenderTarget* RenderProcess::getRenderTarget( uint32_t i )
    {
        if ( i >= 0 )
        {
            if ( i < _render_targets.size() )
                return _render_targets[i].ref();
            else
            {
                KEGE_FATAL( "RenderProcess::getRenderTarget() : INVALID_OPERATION : invalid render target index -> [%i]. Total number of render targets -> #%i.", i, _render_targets.size() );
                throw std::runtime_error("INVALID_OPERATION");
            }
        }
        return nullptr;
    }

    void RenderProcess::executeRenderPhase( RenderPhase* phase )
    {
        phase->begin();
        phase->execute();
        phase->end();

        KEGE::core()->graphics()->renderer()->submit( phase->getCommandBuffer() );
    }

    int RenderProcess::frameIndex()const
    {
        return _frame_index;
    }

    void RenderProcess::execute()
    {
        _frame_index = (_frame_index + 1) % 2;
        
        KEGE::core()->graphics()->renderer()->beginFrame();
        for(kege::Ref< RenderPhase >& phase : _phases )
        {
            executeRenderPhase( phase.ref() );
        }
        KEGE::core()->graphics()->renderer()->endFrame();
    }

    void RenderProcess::present()
    {
        KEGE::core()->graphics()->renderer()->present();
    }

    RenderProcess::~RenderProcess()
    {
        _render_targets.clear();
        _phases.clear();
    }

    RenderProcess::RenderProcess()
    :   _frame_index(-1)
    {}

}
