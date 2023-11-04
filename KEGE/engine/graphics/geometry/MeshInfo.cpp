//
//  MeshInfo.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/13/23.
//

#include "KEGE.hpp"
#include "Core.hpp"
#include "Graphics.hpp"
#include "MeshInfo.hpp"
namespace kege{

    kege::Ref< kege::Geometry > MeshInfo::build()const
    {
        core::VertexArray* vao = nullptr;

        kege::Ref< core::Buffer > vbo = KEGE::core()->graphics()->createBuffer
        (
            core::Buffer::VERTEX,
            _vertex_data.size(),
            _vertex_data.data(),
            core::Buffer::STATIC
        );
        
        std::vector< kege::Vertex::Binding > binding =
        {{
            vbo, _vertex_stride,
            core::VertexInputRate::PER_VERTEX
        }};

        if ( !_index_data.empty() )
        {
            kege::Ref< core::Buffer > ibuffer = KEGE::core()->graphics()->createBuffer
            (
                core::Buffer::INDEX,
                _index_data.size(),
                _index_data.data(),
                core::Buffer::STATIC
            );
            core::IndexBuffer* ibo = new core::IndexBuffer( _index_type, 0, _index_count, ibuffer );
            vao = new core::VertexArrayIndexed( _attributes, binding, ibo );
        }
        else
        {
            vao = new core::VertexArray( _attributes, binding );
        }

        return new kege::Geometry( 0, _index_count, vao );
    }
}
