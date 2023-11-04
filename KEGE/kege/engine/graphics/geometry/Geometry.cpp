//
//  Geometry.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/22/23.
//

#include "../device/core/CommandBuffer.hpp"
#include "../geometry/Geometry.hpp"
namespace kege
{
    Geometry::Geometry
    (
         uint32_t first_index,
         uint32_t draw_count,
         const kege::Ref< core::VertexArray >& vertexarray
    )
    :   _vertexarray( vertexarray )
    ,   _first_index( first_index )
    ,   _draw_count( draw_count )
    {}

    void Geometry::bind( core::CommandBuffer* command )const
    {
        _vertexarray->bind( command );
    }

    void Geometry::draw( core::CommandBuffer* command, uint32_t first_instance, uint32_t instance_count )const
    {
        _vertexarray->draw( command, _first_index, _draw_count, first_instance, instance_count );
    }

    const core::VertexArray* Geometry::vertexArray()const
    {
        return _vertexarray.ref();
    }

    core::VertexArray* Geometry::vertexArray()
    {
        return _vertexarray.ref();
    }

    uint32_t Geometry::firstIndex()const
    {
        return _first_index;
    }
    
    uint32_t Geometry::drawCount()const
    {
        return _draw_count;
    }

    Geometry::~Geometry()
    {
        _vertexarray = nullptr;
    }
}



//namespace kege
//{
//    GeometryIndexed::GeometryIndexed
//    (
//        uint32_t first_index,
//        uint32_t draw_count,
//        const kege::Ref< VertexArray >& vertexarray,
//        const kege::Ref< IndexBuffer >& indexbuffer
//    )
//    :   kege::Geometry( first_index, draw_count, vertexarray )
//    ,   _indexbuffer( indexbuffer )
//    {}
//
//    void GeometryIndexed::bind( core::CommandBuffer* command_buffer )const
//    {
//        kege::Geometry::bind( command_buffer );
//        _indexbuffer->bind( command_buffer );
//    }
//
//    void GeometryIndexed::draw( core::CommandBuffer* command, uint32_t first_instance, uint32_t instance_count )const
//    {
//        int32_t vertex_offset = 0;
//        command->drawIndexed( _draw_count, instance_count, _first_index, vertex_offset, first_instance );
//    }
//
//    const kege::IndexBuffer* GeometryIndexed::indexBuffer()const
//    {
//        return _indexbuffer.ref();
//    }
//
//    kege::IndexBuffer* GeometryIndexed::indexBuffer()
//    {
//        return _indexbuffer.ref();
//    }
//
//    GeometryIndexed:: ~GeometryIndexed()
//    {
//        _indexbuffer.clear();
//    }
//
//}
