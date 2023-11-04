//
//  memory.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 2/3/23.
//

#include "kege-logger.hpp"
#include "memory.hpp"




static const char *strEntityComponentTypes[] = { memory_tag_enums };
const char* string_memory_tag(memory_tag type)
{
    return strEntityComponentTypes[ type ];
};



struct memory_stats
{
    size_t total_allocation;
    size_t tagged_allocations[ _MEMORY_TAG_MAX_COUNT ];
};

static memory_stats _stats;

void* memory_allocate( size_t size, memory_tag tag)
{
    if (tag == _MEMORY_TAG_UNKNOWN)
    {
        KEGE_WARN("using MEMORY_TAG_UNKNOWN, you should re-class this allocation.");
    }
    _stats.total_allocation += size;
    _stats.tagged_allocations[ tag ] += size;
    return malloc( size );
}
void memory_free(void* block, size_t size, memory_tag tag)
{
    if (tag == _MEMORY_TAG_UNKNOWN)
    {
        KEGE_WARN("using MEMORY_TAG_UNKNOWN, you should re-class this allocation.");
    }
    _stats.total_allocation -= size;
    _stats.tagged_allocations[ tag ] -= size;
    free( block );
}
void memory_copy(void* dst, const void* src, size_t size)
{
    memcpy(dst, src, size);
}
void memory_set(void* dst, int value, size_t size)
{
    memset(&_stats, value, sizeof( memory_stats ));
}
void memory_zero(void* dst, size_t size)
{
    memset(dst, 0x0, size);
}

char* memory_usage_get_info()
{
    static const size_t bufrsize = 4096;
    static char buffer[ bufrsize ];
    
    size_t kib = 1024;
    size_t mib = 1024 * kib;
    size_t gib = 1024 * mib;

    size_t offset = 0;
    memset(buffer, 0x0, bufrsize);

    size_t w = snprintf( buffer + offset, bufrsize, "####( System Memory Usage Information )####\n" );
    offset += w;
    size_t total_memory = 0;
    for (int i=0; i<_MEMORY_TAG_MAX_COUNT; i++)
    {
        char unit[4] = "_B";
        float amount = 0.0f;
        if (_stats.tagged_allocations[i] >= gib)
        {
            unit[0] = 'G';
            amount = _stats.tagged_allocations[i] / (float)gib;
        }
        else if (_stats.tagged_allocations[i] >= mib)
        {
            unit[0] = 'M';
            amount = _stats.tagged_allocations[i] / (float)mib;
        }
        else if (_stats.tagged_allocations[i] >= kib)
        {
            unit[0] = 'K';
            amount = _stats.tagged_allocations[i] / (float)kib;
        }
        else
        {
            unit[0] = 0;
            unit[1] = 0;
            amount = _stats.tagged_allocations[i];
        }
        size_t w = snprintf( buffer + offset, bufrsize - offset, "%25s: %8.2f %s\n", string_memory_tag( (memory_tag)i ), amount, unit );
        offset += w;

        total_memory += _stats.tagged_allocations[i];
    }

    char unit[4] = "_B";
    if (total_memory >= gib)
    {
        unit[0] = 'G';
        snprintf( buffer + offset, bufrsize - offset, "%25s: %8.2f %s\n\n", "Total Memory Usage", total_memory / (float)gib, unit );
    }
    else if (total_memory >= mib)
    {
        unit[0] = 'M';
        snprintf( buffer + offset, bufrsize - offset, "%25s: %8.2f %s\n\n", "Total Memory Usage", total_memory / (float)mib, unit );
    }
    else if (total_memory >= kib)
    {
        unit[0] = 'K';
        snprintf( buffer + offset, bufrsize - offset, "%25s: %8.2f %s\n\n", "Total Memory Usage", total_memory / (float)kib, unit );
    }
    else
    {
        unit[0] = 0;
        unit[1] = 0;
        snprintf( buffer + offset, bufrsize - offset, "%25s: %8.2f %s\n\n", "Total Memory Usage", (float)total_memory, unit );
    }
    return buffer;
}

void memory_initialize()
{
    memset(&_stats, 0x0, sizeof( memory_stats ));
}
void memory_shutdown()
{

}




//template<typename T> T* memory_alloc_a( size_t count, memory_tag tag )
//{
//    if (tag == _MEMORY_TAG_UNKNOWN)
//    {
//        KEGE_WARN("using MEMORY_TAG_UNKNOWN, you should re-class this allocation.");
//    }
//    size_t size = sizeof( T ) * count;
//    _stats.total_allocation += size;
//    _stats.tagged_allocations[ tag ] += size;
//    return new T[ count ];
//}
//template<typename T> void memory_free_a( T* t, size_t count, memory_tag tag )
//{
//    if (tag == _MEMORY_TAG_UNKNOWN)
//    {
//        KEGE_WARN("using MEMORY_TAG_UNKNOWN, you should re-class this allocation.");
//    }
//    size_t size = sizeof( T ) * count;
//    _stats.total_allocation -= size;
//    _stats.tagged_allocations[ tag ] -= size;
//    delete [] t;
//}template<typename T> void memory_free( T* t, memory_tag tag )
//{
//    if (tag == _MEMORY_TAG_UNKNOWN)
//    {
//        KEGE_WARN("using MEMORY_TAG_UNKNOWN, you should re-class this allocation.");
//    }
//    size_t size = sizeof( T );
//    _stats.total_allocation -= size;
//    _stats.tagged_allocations[ tag ] -= size;
//    delete t;
//}
//template<typename T> T* memory_alloc( memory_tag tag )
//{
//    if (tag == _MEMORY_TAG_UNKNOWN)
//    {
//        KEGE_WARN("using MEMORY_TAG_UNKNOWN, you should re-class this allocation.");
//    }
//    size_t size = sizeof( T );
//    _stats.total_allocation += size;
//    _stats.tagged_allocations[ tag ] += size;
//    return new T;
//}



