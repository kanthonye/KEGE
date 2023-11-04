//
//  Renderer.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 7/6/23.
//

#ifndef KEGE_RENDERER_HPP
#define KEGE_RENDERER_HPP

#include "../renderer/RenderLayers.hpp"

namespace kege
{
    class Renderer : public kege::RefCounter
    {
    public:

        void insert( uint32_t render_pass, uint32_t layer, const kege::ObjectParameter& objparam, const kege::Ref< kege::Geometry >& geometry, const kege::Ref< kege::Material >& material );
        void createRenderProcess( uint32_t render_pass, kege::core::CommandBuffer* command_buffer );
        void insertRenderTarget( uint32_t render_pass, kege::core::Framebuffer* framebuffer );
        void setCamera( const kege::mat44& transform, const kege::mat44& projection );
        void execute( );
        void begin( );
        void end( );
        void clear();
        ~Renderer();
        Renderer();

    private:

        std::map< uint32_t, kege::Ref< kege::RenderLayers > > _render_processes;
        friend RenderLayers;
    };
}
#endif /* KEGE_RENDER_HPP */
