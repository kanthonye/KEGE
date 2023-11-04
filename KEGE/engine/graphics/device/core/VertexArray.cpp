//
//  VertexArray.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/9/23.
//

#include "VertexArray.hpp"
#include "../../device/core/CommandBuffer.hpp"
namespace kege{namespace core{

    VertexArray::VertexArray
    (
        const std::vector< kege::Vertex::Attribute >& attributes,
        const std::vector< kege::Vertex::Binding >& bindings
    )
    :   _attributes( attributes )
    ,   _bindings( bindings )
    {}

    void VertexArray::draw( core::CommandBuffer* command, uint32_t first_vertex, uint32_t vertex_count, uint32_t first_instance, uint32_t instance_count )const
    {
        //command->draw( vertex_count, instance_count, first_vertex, first_instance );
    }
    
    void VertexArray::bind( core::CommandBuffer* command_buffer )const
    {
        for ( auto& binding : _bindings )
        {
            command_buffer->bindVertexBuffer( binding._buffer.ref() );
        }
    }

    const std::vector< kege::Vertex::Attribute >& VertexArray::attributes()const
    {
        return _attributes;
    }

    const std::vector< kege::Vertex::Binding >& VertexArray::bindings()const
    {
        return _bindings;
    }

    void VertexArray::addAttribute( const kege::Vertex::Attribute& attribute )
    {
        _attributes.push_back( attribute );
    }

    void VertexArray::addBinding( const kege::Vertex::Binding& binding )
    {
        _bindings.push_back( binding );
    }

    VertexArray::~VertexArray()
    {
        _bindings.clear();
        _attributes.clear();
    }

}}


namespace kege{namespace core{

    VertexArrayIndexed::VertexArrayIndexed
    (
        const std::vector< kege::Vertex::Attribute >& attributes,
        const std::vector< kege::Vertex::Binding >& bindings,
        const kege::Ref< core::IndexBuffer >& indexbuffer
    )
    :   core::VertexArray( attributes, bindings )
    ,   _indexbuffer( indexbuffer )
    {}

    void VertexArrayIndexed::bind( core::CommandBuffer* command_buffer )const
    {
        core::VertexArray::bind( command_buffer );
        _indexbuffer->bind( command_buffer );
    }

    void VertexArrayIndexed::draw( core::CommandBuffer* command, uint32_t first_vertex, uint32_t vertex_count, uint32_t first_instance, uint32_t instance_count )const
    {
        int32_t vertex_offset = 0;
        command->drawIndexed( vertex_count, instance_count, first_vertex, vertex_offset, first_instance );
    }

    const core::IndexBuffer* VertexArrayIndexed::indexBuffer()const
    {
        return _indexbuffer.ref();
    }

    core::IndexBuffer* VertexArrayIndexed::indexBuffer()
    {
        return _indexbuffer.ref();
    }

    VertexArrayIndexed:: ~VertexArrayIndexed()
    {
        _indexbuffer.clear();
    }

}}
