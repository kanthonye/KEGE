//
//  PoolIndex.cpp
//  PoolSystem
//
//  Created by Kenneth Esdaile on 3/12/23.
//

#include "PoolIndex.hpp"
namespace kege
{

#ifdef POOL_INDEX_8_BIT_INTEGER

//    PoolIndex& PoolIndex::operator =(int16_t value)
//    {
//        _pool_id = (uint8_t)(value >> 8);
//        _cell_id = (uint8_t)(value & 0xFF);
//        return *this;
//    }
//    PoolIndex::operator int16_t() const
//    {
//        return (((uint16_t)_pool_id) << 8) | _cell_id;
//    }

#elif POOL_INDEX_16_BIT_INTEGER

//    PoolIndex& PoolIndex::operator =(int32_t value)
//    {
//        _pool_id = (uint16_t)(value >> 16);
//        _cell_id = (uint16_t)(value & 0xFFFF);
//        return *this;
//    }
//    PoolIndex::operator int32_t() const
//    {
//        return (((uint16_t)_pool_id) << 16) | _cell_id;
//    }

#else

//    PoolIndex& PoolIndex::operator =(int64_t value)
//    {
//        _pool_id = (uint32_t)(value >> 32);
//        _cell_id = (uint32_t)(value & 0xFFFFFFFF);
//        return *this;
//    }
//    PoolIndex::operator int64_t() const
//    {
//        return (((uint64_t)_pool_id) << 32) | _cell_id;
//    }
    
#endif
}
