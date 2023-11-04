//
//  RenderPhase.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/24/23.
//

#include "../render-process/RenderPhase.hpp"
#include "../render-process/RenderProcess.hpp"
#include "../render-process/RenderTarget.hpp"
namespace kege
{
    RenderPhase::RenderPhase( int render_target, int input_target )
    :   _current_target_index( render_target )
    ,   _render_target( nullptr )
    ,   _input_target_index( input_target )
    ,   _process( nullptr )
    ,   _subpass_contents( core::SUBPASS_CONTENTS_PRIMARY_COMMAND_BUFFERS )
    {}
    
    void RenderPhase::begin()
    {
        /**
         * Begin the recording of the primary command buffer
         */
        getCommandBuffer()->begin();

        /**
         * Get the render target this phase should render too. Some phase my not
         * need to bind a render target, so we need to check for null.
         * Note: render target can changes at runtime
         */
        _render_target = _process->getRenderTarget( _current_target_index );
        if ( _render_target )
        {
            _render_target->beginRenderPass( getCommandBuffer(), _subpass_contents );
        }

        _input_target = _process->getRenderTarget( _input_target_index );
    }
    
    kege::RenderTarget* RenderPhase::getCurrentRenderTarget()
    {
        return _render_target;
    }
    
    kege::RenderTarget* RenderPhase::getInputRenderTarget()
    {
        return _input_target;
    }

    void RenderPhase::end()
    {
        /**
         * End the writing to the curren render target.
         */
        if ( _render_target )
        {
            _render_target->endRenderPass( getCommandBuffer() );
        }

        /**
         end primary command buffers recording.
         */
        getCommandBuffer()->end();
    }

    void RenderPhase::execute()
    {}

    void RenderPhase::setRenderTargetIndex( int index )
    {
        _current_target_index = index;
    }
    
    RenderPhase::~RenderPhase()
    {}

}
