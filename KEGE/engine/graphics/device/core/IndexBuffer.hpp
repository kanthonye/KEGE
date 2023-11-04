//
//  IndexBuffer.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/9/23.
//

#ifndef KEGE_INDEX_ARRAY_HPP
#define KEGE_INDEX_ARRAY_HPP

#include "Ref.hpp"
#include "cgm.hpp"
#include "../../device/core/Buffer.hpp"

namespace kege{namespace core{
    class CommandBuffer;
    
    class IndexBuffer : public kege::RefCounter
    {
    public:

        IndexBuffer
        (
             core::Index::Type index_type,
             uint32_t first_index,
             uint32_t index_count,
            const kege::Ref< core::Buffer >& buffer
        );
        
        void bind( core::CommandBuffer* command_buffer )const;

        core::Index::Type indexType()const;
        uint32_t firstIndex()const;
        uint32_t indexCount()const;
        core::Buffer* buffers();
        ~IndexBuffer();

    protected:

        IndexBuffer& operator=( const IndexBuffer& ) = default;
        IndexBuffer( const IndexBuffer& ) = default;
        IndexBuffer() = default;

    protected:

        kege::Ref< core::Buffer > _buffer;
        uint32_t _first_index;
        uint32_t _index_count;
        core::Index::Type _index_type;
    };
}}
#endif /* IndexBuffer_hpp */
