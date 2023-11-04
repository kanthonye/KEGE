//
//  Geometry.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/22/23.
//

#ifndef KEGE_GEOMETRY_HPP
#define KEGE_GEOMETRY_HPP

#include "../device/core/VertexArray.hpp"

namespace kege
{
    class Geometry : public kege::RefCounter
    {
    public:

        Geometry
        (
             uint32_t first_index,
             uint32_t draw_count,
             const kege::Ref< core::VertexArray >& vertexarray
        );

        void operator()
        (
            uint32_t first_index,
            uint32_t draw_count,
            const kege::Ref< core::VertexArray >& vertexarray
        );

        virtual void draw( core::CommandBuffer* command, uint32_t first_instance, uint32_t instance_count )const;
        virtual void bind( core::CommandBuffer* command_buffer )const;

        const core::VertexArray* vertexArray()const;
        core::VertexArray* vertexArray();

        uint32_t firstIndex()const;
        uint32_t drawCount()const;

        virtual ~Geometry();

    protected:

        Geometry& operator=( const Geometry& ) = default;
        Geometry( const Geometry& ) = default;
        Geometry() = default;

    protected:

        kege::Ref< core::VertexArray > _vertexarray;
        uint32_t _first_index;
        uint32_t _draw_count;
    };
}


//
//namespace kege
//{
//    class GeometryIndexed : public kege::Geometry
//    {
//    public:
//
//        GeometryIndexed
//        (
//            uint32_t first_index,
//            uint32_t index_count,
//            const kege::Ref< VertexArray >& vertexarray,
//            const kege::Ref< IndexBuffer >& indexbuffer
//        );
//
//        virtual void draw( core::CommandBuffer* command, uint32_t first_instance, uint32_t instance_count )const;
//        virtual void bind( core::CommandBuffer* command )const;
//
//        const kege::IndexBuffer* indexBuffer()const;
//        kege::IndexBuffer* indexBuffer();
//
//        virtual ~GeometryIndexed();
//
//    protected:
//
//        GeometryIndexed& operator=( const GeometryIndexed& ) = default;
//        GeometryIndexed( const GeometryIndexed& ) = default;
//        GeometryIndexed() = default;
//
//    protected:
//
//        kege::Ref< kege::IndexBuffer > _indexbuffer;
//    };
//}
#endif /* KEGE_GEOMETRY_HPP */
