//
//  memory.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 2/3/23.
//

#ifndef memory_hpp
#define memory_hpp

#include <vector>
#include <cstring>

#define memory_tag_enums \
memory_tag_(MEMORY_TAG_UNKNOWN),\
memory_tag_(MEMORY_TAG_ARRAY),\
memory_tag_(MEMORY_TAG_DARRAY),\
memory_tag_(MEMORY_TAG_STRING),\
memory_tag_(MEMORY_TAG_APPLICATION),\
memory_tag_(MEMORY_TAG_TEXTURE),\
memory_tag_(MEMORY_TAG_MATERIAL),\
memory_tag_(MEMORY_TAG_RENDERER),\
memory_tag_(MEMORY_TAG_TRANSFORM),\
memory_tag_(MEMORY_TAG_ENTITY),\
memory_tag_(MEMORY_TAG_ENTITY_NODE),\
memory_tag_(MEMORY_TAG_SCENE),\
memory_tag_(MEMORY_TAG_JOB),\
memory_tag_(MEMORY_TAG_DIC),\
memory_tag_(MEMORY_TAG_RING_QUEUE),\
memory_tag_(MEMORY_TAG_BTS),\
memory_tag_(MEMORY_TAG_MAX_COUNT),\

#define memory_tag_(x) _##x

typedef enum memory_tag{
    memory_tag_enums
//    MEMORY_TAG_UNKNOWN,
//    MEMORY_TAG_ARRAY,
//    MEMORY_TAG_DARRAY,
//    MEMORY_TAG_STRING,
//    MEMORY_TAG_APPLICATION,
//    MEMORY_TAG_TEXTURE,
//    MEMORY_TAG_MATERIAL,
//    MEMORY_TAG_RENDERER,
//    MEMORY_TAG_TRANSFORM,
//    MEMORY_TAG_ENTITY,
//    MEMORY_TAG_ENTITY_NODE,
//    MEMORY_TAG_SCENE,
//    MEMORY_TAG_JOB,
//    MEMORY_TAG_DIC,
//    MEMORY_TAG_RING_QUEUE,
//    MEMORY_TAG_BTS,
//
//    MEMORY_TAG_MAX_COUNT,
} memory_tag;

#undef  memory_tag_
#define memory_tag_(x) #x

void* memory_allocate( size_t size, memory_tag tag);
void memory_free(void* block, size_t size, memory_tag tag);
void memory_copy(void* dst, const void* src, size_t size);
void memory_set(void* dst, int value, size_t size);
void memory_zero(void* dst, size_t size);
void memory_initialize();
void memory_shutdown();

char* memory_usage_get_info();


template<typename T> T* mem_alloc( memory_tag usage )
{
    return (T*) memory_allocate( sizeof( T ) , usage );
}

template<typename T> void mem_free( T* t, memory_tag usage )
{
    t->~T();
    memory_free(t, sizeof(T), usage );
}

#endif /* memory_hpp */
