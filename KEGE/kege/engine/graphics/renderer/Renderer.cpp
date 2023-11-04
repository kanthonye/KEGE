//
//  Renderer.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 7/6/23.
//
#include "../renderer/Renderer.hpp"
#include "../../app/Core.hpp"
#include "../../app/KEGE.hpp"
#include "../../app/Graphics.hpp"

namespace kege
{
    void Renderer::insert( uint32_t render_pass, uint32_t layer, const kege::ObjectParameter& objparam, const kege::Ref< kege::Geometry >& geometry, const kege::Ref< kege::Material >& material )
    {
        _render_processes[ render_pass ]->insert( layer, objparam, geometry, material );
    }
    void Renderer::createRenderProcess( uint32_t render_pass, kege::core::CommandBuffer* command_buffer )
    {
        kege::RenderLayers* process = new kege::RenderLayers( render_pass, command_buffer );
        _render_processes[ render_pass ] = process;
        process->_renderer = this;
    }
    void Renderer::insertRenderTarget( uint32_t render_pass, kege::core::Framebuffer* framebuffer )
    {
        //_render_targets[ render_pass ] = framebuffer;
    }
    
    void Renderer::setCamera( const kege::mat44& transform, const kege::mat44& projection )
    {
//        _global_parameters._projection = projection;
//        _global_parameters._transform = transform;
    }

    void Renderer::execute()
    {
//        _global_resource.target()->getBuffer()->copyToSystemMemory( 0, sizeof( GlobalParameter ), &_global_parameters );
//        for ( auto& m : _render_processes )
//        {
//            m.second->begin( );
//            m.second->execute( );
//            m.second->end( );
//        }
    }

    void Renderer::begin()
    {
    }

    void Renderer::end()
    {
        for ( auto& m : _render_processes )
        {
            m.second->clear();
        }
    }

    void Renderer::clear()
    {
        _render_processes.clear();
    }

    Renderer::~Renderer()
    {
//        _global_resource.destruct();
//        clear();
    }

    Renderer::Renderer()
    {
//        kege::Ref< core::Buffer > buffer = KEGE::core()->graphics()->createBuffer
//        (
//            core::Buffer::UNIFORM,
//            sizeof( GlobalParameter ),
//            &_global_parameters,
//            core::Buffer::STREAM
//        );
//        _global_resource.setTarget( new core::UniformTargetBuffer( 0,0,buffer ) );
    }
}



namespace kege
{

}
