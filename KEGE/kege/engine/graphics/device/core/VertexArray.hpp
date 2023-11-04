//
//  VertexArray.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/9/23.
//

#ifndef KEGE_VERTEX_ARRAY_HPP
#define KEGE_VERTEX_ARRAY_HPP

#include "../../device/core/Vertex.hpp"
#include "../../device/core/IndexBuffer.hpp"

namespace kege{namespace core{

    class VertexArray : public kege::RefCounter
    {
    public:

        VertexArray
        (
            const std::vector< kege::Vertex::Attribute >& attributes,
            const std::vector< kege::Vertex::Binding >& bindings
        );

        virtual void draw( core::CommandBuffer* command, uint32_t first_vertex, uint32_t vertex_count, uint32_t first_instance, uint32_t instance_count )const;
        virtual void bind( core::CommandBuffer* command_buffer )const;

        const std::vector< kege::Vertex::Attribute >& attributes()const;
        const std::vector< kege::Vertex::Binding >& bindings()const;

        void addAttribute( const Vertex::Attribute& attribute );
        void addBinding( const Vertex::Binding& binding );

        virtual ~VertexArray();

    protected:

        VertexArray& operator=( const VertexArray& ) = default;
        VertexArray( const VertexArray& ) = default;
        VertexArray() = default;

    protected:

        std::vector< kege::Vertex::Attribute > _attributes;
        std::vector< kege::Vertex::Binding > _bindings;
    };

}}

namespace kege{namespace core{

    class VertexArrayIndexed : public core::VertexArray
    {
    public:

        VertexArrayIndexed
        (
            const std::vector< kege::Vertex::Attribute >& attributes,
            const std::vector< kege::Vertex::Binding >& bindings,
            const kege::Ref< core::IndexBuffer >& indexbuffer
        );

        void draw( core::CommandBuffer* command, uint32_t first_vertex, uint32_t vertex_count, uint32_t first_instance, uint32_t instance_count )const;
        void bind( core::CommandBuffer* command )const;

        const core::IndexBuffer* indexBuffer()const;
        core::IndexBuffer* indexBuffer();

        virtual ~VertexArrayIndexed();

    protected:

        VertexArrayIndexed& operator=( const VertexArrayIndexed& ) = default;
        VertexArrayIndexed( const VertexArrayIndexed& ) = default;
        VertexArrayIndexed() = default;

    protected:

        kege::Ref< core::IndexBuffer > _indexbuffer;
    };

}}
#endif /* KEGE_VERTEX_ARRAY_HPP */
