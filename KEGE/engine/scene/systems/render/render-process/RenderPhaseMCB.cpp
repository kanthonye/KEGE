//
//  RenderPhaseMCB.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/8/23.
//

#include "../render-process/RenderProcess.hpp"
#include "../render-process/RenderPhaseMCB.hpp"

namespace kege
{
    RenderPhaseMCBElement::RenderPhaseMCBElement
    (
        kege::Ref< core::CommandBuffer > commands[2],
        kege::Ref< kege::RenderTechnique > technique
    )
    :   _command_buffers{ commands[0], commands[1] }
    ,   _technique( technique )
    {}

    core::CommandBuffer* RenderPhaseMCBElement::getCommandBuffer( uint32_t frame_index )
    {
        return _command_buffers[ frame_index ].ref();
    }

    kege::RenderTechnique* RenderPhaseMCBElement::getRenderTechnique()
    {
        return _technique.ref();
    }

    RenderPhaseMCBElement::~RenderPhaseMCBElement()
    {
        _command_buffers[0].clear();
        _command_buffers[1].clear();
        _technique.clear();
    }

}


namespace kege
{
    RenderPhaseMCB::RenderPhaseMCB( int render_target, int input_target, const std::vector< kege::Ref< RenderTechnique > >& techniques )
    :   kege::RenderPhase( render_target, input_target )
    {
        /* Specifies that the render commands will be provided into the secondary command buffer. */
        _subpass_contents = core::SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS;

        /* create the primary buffer to be used with this render pahse. */
        _commands[0] = KEGE::core()->graphics()->device()->createCommandBuffer( core::GRAPHICS_QUEUE );
        _commands[1] = KEGE::core()->graphics()->device()->createCommandBuffer( core::GRAPHICS_QUEUE );

        /**
         * create the secondary buffer for clearing the render-target because it
         * can not be done by the primary command buffer due to the fact that it
         * using secondary command buffers.
         *
         * @see RenderPhaseMCB::execute()
         * */
        _clear_comm_bufrs[0] = _commands[0]->createSecondaryCommandBuffer();
        _clear_comm_bufrs[1] = _commands[1]->createSecondaryCommandBuffer();

        /* insert the render techniques. */
        setRenderTechniques( techniques );
    }

    void RenderPhaseMCB::setRenderTechniques( const std::vector< kege::Ref< RenderTechnique > >& techniques )
    {
        for (auto& technique : techniques)
        {
            addRenderTechniques( technique );
        }
    }

    void RenderPhaseMCB::addRenderTechniques( const kege::Ref< RenderTechnique >& technique )
    {
        /* create the secondary buffer for the render technique. */
        kege::Ref< core::CommandBuffer > commands[2] =
        {
            _commands[0]->createSecondaryCommandBuffer(),
            _commands[1]->createSecondaryCommandBuffer()
        };

        /* add render phase element to the _elements container. */
        _elements.push_back({ commands, technique });
        _elements[ _elements.size() - 1 ].getRenderTechnique()->setRenderPhase( this );
    }

    core::CommandBuffer* RenderPhaseMCB::getCommandBuffer()
    {
        return _commands[ _process->frameIndex() ].ref();
    }

    void RenderPhaseMCB::execute()
    {
        const core::Framebuffer* framebuffer = _render_target->getFramebuffer();
        const uint HEIGHT = framebuffer->height();
        const uint WIDTH = framebuffer->width();

        kege::RenderTechnique* technique;
        core::CommandBuffer* command;
        std::vector< core::CommandBuffer* > secondary_command_buffers;

        command = _clear_comm_bufrs[ _process->frameIndex() ].ref();
        command->begin();
        _render_target->clearAttachments( command );
        command->end();
        secondary_command_buffers.push_back( command );

        for(auto& element : _elements )
        {
            technique = element.getRenderTechnique();
            command = element.getCommandBuffer( _process->frameIndex() );

            command->begin();
            command->setScissor( 0, 0, WIDTH, HEIGHT );
            command->setViewport( 0, 0, WIDTH, HEIGHT );
            technique->execute( command );
            command->end();

            secondary_command_buffers.push_back( command );
        }

        /* execute the secondary command buffers. */
        getCommandBuffer()->executeCommands( secondary_command_buffers );

    }

    RenderPhaseMCB::~RenderPhaseMCB()
    {
        _elements.clear();
        _clear_comm_bufrs[0].clear();
        _clear_comm_bufrs[1].clear();
        _commands[0].clear();
        _commands[1].clear();
    }

}
