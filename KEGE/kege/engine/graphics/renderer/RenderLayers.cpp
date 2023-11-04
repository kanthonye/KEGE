//
//  RenderProcess.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/10/23.
//

#include "KEGE.hpp"
#include "Core.hpp"
#include "Graphics.hpp"

#include "Renderer.hpp"
#include "RenderLayers.hpp"
namespace kege
{
    void RenderLayers::insert( uint32_t layer, const kege::ObjectParameter& objparam, const kege::Ref< kege::Geometry >& geometry, const kege::Ref< kege::Material >& material )
    {
        RenderLayer* render_layer;
        auto m = _layers.find( layer );
        if ( m == _layers.end() )
        {
            render_layer = new RenderLayer;
            render_layer->_process = this;
            _layers[ layer ] = render_layer;
        }
        else
        {
            render_layer = m->second.ref();
        }

        for ( const kege::Ref< kege::ShaderEffect >& effect : material->effects() )
        {
            render_layer->insert( objparam, geometry, effect );
        }
    }
    void RenderLayers::execute( )
    {
//        _command->bindUniformLayout( 0, 0, &_renderer->_global_resource );
//        for ( auto& m : _layers )
//        {
//            m.second->execute( _command );
//        }
    }

    void RenderLayers::begin( )
    {
        //_command->reset( context );
        //_command->beginRecording( core::COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT );
        //_command->beginRenderpass( context->framebuffer() );
    }

    void RenderLayers::end( )
    {
        //_command->endRenderpass();
        //_command->endRecording();
        //context->submit( _command );
    }

    void RenderLayers::clear()
    {
        _layers.clear();
    }

    RenderLayers::RenderLayers( uint32_t render_pass, core::CommandBuffer* command )
    :   _command( command )
    ,   _render_pass( render_pass )
    {
        if ( _command == nullptr )
        {
            KEGE_FATAL( "INVALID_OPERTION: passing nullptr core::CommandBuffer to RenderProcess constructor not allowed." );
        }
    }
    RenderLayers::~RenderLayers()
    {
        clear();
    }
}
