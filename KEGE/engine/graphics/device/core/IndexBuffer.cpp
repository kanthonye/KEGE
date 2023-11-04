//
//  IndexBuffer.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/9/23.
//

#include "../../device/core/IndexBuffer.hpp"
#include "../../device/core/CommandBuffer.hpp"

namespace kege{namespace core{
    
    IndexBuffer::IndexBuffer
    (
         core::Index::Type index_type,
         uint32_t first_index,
         uint32_t index_count,
         const kege::Ref< core::Buffer >& buffer
    )
    :   _buffer( buffer )
    ,   _first_index( first_index )
    ,   _index_count( index_count )
    ,   _index_type( index_type )
    {}

    void IndexBuffer::bind( core::CommandBuffer* command_buffer )const
    {
        command_buffer->bindIndexBuffer( _buffer.ref(), _index_type );
    }

    core::Index::Type IndexBuffer::indexType()const
    {
        return _index_type;
    }

    uint32_t IndexBuffer::firstIndex()const
    {
        return _first_index;
    }

    uint32_t IndexBuffer::indexCount()const
    {
        return _index_count;
    }

    core::Buffer* IndexBuffer::buffers()
    {
        return _buffer.ref();
    }
    
    IndexBuffer::~IndexBuffer()
    {
        if ( _buffer )
        {
            _buffer->destruct();
            _buffer = nullptr;
        }
    }

}}
