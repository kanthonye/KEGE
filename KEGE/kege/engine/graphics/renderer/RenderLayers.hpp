//
//  RenderProcess.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/10/23.
//

#ifndef KEGE_RENDER_LAYERS_HPP
#define KEGE_RENDER_LAYERS_HPP

#include "../renderer/RenderLayer.hpp"

namespace kege
{
    class RenderLayers : public kege::RefCounter
    {
    public:

        void insert( uint32_t layer, const kege::ObjectParameter& objparam, const kege::Ref< kege::Geometry >& geometry, const kege::Ref< kege::Material >& material );
        RenderLayers( uint32_t render_pass, core::CommandBuffer* command_buffer );

        void execute( );
        void begin( );
        void end( );
        core::CommandBuffer* commandBuffer();
        void clear();
        
        ~RenderLayers();

    private:

        std::map< uint32_t, kege::Ref< kege::RenderLayer > > _layers;
        core::CommandBuffer* _command;
        kege::Renderer* _renderer;
        uint32_t _render_pass;
        
        friend kege::RenderLayer;
        friend kege::Renderer;
    };
}
#endif /* KEGE_RENDER_PROCESS_HPP */
