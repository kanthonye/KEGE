//
//  Buffer.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/11/23.
//

#ifndef KEGE_CORE_BUFFER
#define KEGE_CORE_BUFFER
#include "Ref.hpp"
#include <stdlib.h>
#include "../../device/core/Abstraction.h"
#include "../../device/core/graphics-types.hpp"
namespace kege{namespace vk{
    class Buffer;
}}
namespace kege{namespace core{

    class CommandBuffer;

    class Buffer : public kege::RefCounter
    {
    public:

        enum Type
        {
            VERTEX,
            INDEX,
            UNIFORM,
            STORAGE,
            UNIFORM_TEXEL,
            STORAGE_TEXEL,
            TRANSFER,
        };

        enum Usage
        {
            STATIC,
            DYNAMIC,
            STREAM,
            STAGE,
        };

    public:

        virtual void destruct() = 0;

        virtual void copyFromSystemMemory( uint64_t offset, uint64_t size, void*& data ){}
        virtual void copyFromSystemMemory( uint64_t size, void*& data ){}
        virtual void copyFromSystemMemory( void*& data ){}

        virtual void copyToSystemMemory( uint64_t offset, uint64_t size, const void* data ){}
        virtual void copyToSystemMemory( uint64_t size, const void* data ){}

        virtual const void* mapMemory()const{ return nullptr; }
        virtual void* mapMemory(){ return nullptr; }
        virtual void unmapMemory(){}

        virtual uint64_t sizeInBytes()const{ return 0; }

        virtual const vk::Buffer* vulkan()const{ return nullptr; }
        virtual vk::Buffer* vulkan(){ return nullptr; }

        KEGE_CORE_ABSTRACTION( Buffer );
    };




    struct Index
    {
        enum Type{ UI8, UI16, UI32,};
    };
}}
#endif /* KEGE_CORE_BUFFER */
