//
//  RenderGroup.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/10/23.
//

#include "../renderer/RenderGroup.hpp"
#include "../renderer/EffectsGroup.hpp"
namespace kege
{
    void RenderGroup::insert( const kege::ObjectParameter& param )
    {
        _elements.push_back( param );
    }
    
    void RenderGroup::execute( core::CommandBuffer* command, const kege::Geometry* geometry )
    {
        for ( ObjectParameter& param : _elements )
        {
            //command->pushConstants( core::Shader::VERTEX, 0, sizeof( ObjectParameter ), &param );
            //geometry->draw( command, 0, 1 );
        }

//        if ( vao->indexbuffer() )
//        {
//            int32_t vertex_offset = 0;
//            for ( ObjectParameter& param : _elements )
//            {
//                command->pushConstants( core::Shader::VERTEX, 0, sizeof( ObjectParameter ), &param );
//                //command->draw( 3, 1, 0, 0 );
//                command->drawIndexed( state->_geometry->indexCount(), instance_count, state->_geometry->firstIndex(), vertex_offset, first_instance );
//            }
//        }
//        else
//        {
//            for ( ObjectParameter& param : _elements )
//            {
//                command->pushConstants( core::Shader::VERTEX, 0, sizeof( ObjectParameter ), &param );
//                //command->draw( 3, 1, 0, 0 );
//                command->draw( state->_geometry->indexCount(), instance_count, state->_geometry->firstIndex(), first_instance );
//            }
//        }
    }

    void RenderGroup::clear()
    {
        _elements.clear();
    }

    RenderGroup::~RenderGroup()
    {
        clear();
    }

    RenderGroup::RenderGroup()
    : _effect( nullptr )
    {}
}


