//
//  MainRenderPhase.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/8/23.
//

#include "../render-process/RenderPhase.hpp"
#include "../render-process/RenderProcess.hpp"
#include "../render-process/RenderTarget.hpp"
#include "../render-process/MainRenderPhase.hpp"
namespace kege
{
    MainRenderPhase::MainRenderPhase( int render_target, int input_target, const std::vector< kege::Ref< RenderTechnique > >& techniques )
    :   RenderPhase( render_target, input_target )
    ,   _techniques( techniques )
    {
        /* Specifies that the render commands will be provided into the primary command buffer. */
        _subpass_contents = core::SUBPASS_CONTENTS_PRIMARY_COMMAND_BUFFERS;

        for (auto& technique : _techniques)
        {
            technique->setRenderPhase( this );
        }
    }

    void MainRenderPhase::add( const kege::Ref< RenderTechnique >& technique )
    {
        _techniques.push_back( technique );
        _techniques[ _techniques.size() - 1 ]->setRenderPhase( this );
    }

    core::CommandBuffer* MainRenderPhase::getCommandBuffer()
    {
        return KEGE::core()->graphics()->renderer()->getCurrentCommandBuffer();
    }

    void MainRenderPhase::execute()
    {
        core::CommandBuffer* command = getCommandBuffer();
        for(auto& technique : _techniques )
        {
            technique->execute( command );
        }
    }

    void MainRenderPhase::begin()
    {
        RenderPhase::begin();
        _render_target->clearAttachments( getCommandBuffer() );
    }

    MainRenderPhase::~MainRenderPhase()
    {
        _techniques.clear();
    }
}
