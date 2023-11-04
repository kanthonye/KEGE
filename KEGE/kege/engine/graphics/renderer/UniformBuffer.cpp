//
//  UniformBuffer.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/12/23.
//

#include "UniformBuffer.hpp"
namespace kege
{
    UniformBuffer::UniformBuffer( uint64_t size, const void* data )
    {}

    void UniformBuffer::bind( core::CommandBuffer* command )const
    {

    }

    kege::Buffer UniformBuffer::buffers()
    {
        return nullptr;
    }
    UniformBuffer::~UniformBuffer()
    {

    }
}
