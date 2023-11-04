//
//  UniformBuffer.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/12/23.
//

#ifndef KEGE_UNIFORM_BUFFER_HPP
#define KEGE_UNIFORM_BUFFER_HPP
#include "Ref.hpp"
#include "cgm.hpp"
#include "../device/core/Buffer.hpp"
namespace kege
{
    typedef core::Buffer* Buffer;

    class UniformBuffer : public kege::RefCounter
    {
    public:

        UniformBuffer( uint64_t size, const void* data );

        void bind( core::CommandBuffer* command )const;

        kege::Buffer buffers();
        ~UniformBuffer();

    protected:

        UniformBuffer& operator=( const UniformBuffer& ) = default;
        UniformBuffer( const UniformBuffer& ) = default;
        UniformBuffer() = default;

    protected:

        kege::Buffer _buffer[ 2 ];
    };

}
#endif /* UniformBuffer_hpp */
