//
//  PoolIndex.hpp
//  PoolSystem
//
//  Created by Kenneth Esdaile on 3/12/23.
//

#ifndef PoolIndex_hpp
#define PoolIndex_hpp
#define KEGE_INVALID_POOL_INDEX uint32_t(-1)
#include <stdlib.h>
#include <ostream>
namespace kege
{

    typedef struct PoolIndex
    {
        friend std::ostream& operator <<(std::ostream& s, const PoolIndex& pi )
        {
            return ( s <<"pool: " << pi._pool_id <<", cell: " << pi._cell_id );
        }
        friend bool operator ==(const PoolIndex& a, const PoolIndex& b)
        {
            return a._pool_id == b._pool_id && a._cell_id == b._cell_id;
        }

        friend bool operator !=(const PoolIndex& a, const PoolIndex& b)
        {
            return a._pool_id != b._pool_id || a._cell_id != b._cell_id;
        }

        friend bool operator <(const PoolIndex& a, const PoolIndex& b)
        {
            return a._pool_id <= b._pool_id && a._cell_id < b._cell_id;
        }

        friend bool operator >(const PoolIndex& a, const PoolIndex& b)
        {
            return a._pool_id >= b._pool_id && a._cell_id > b._cell_id;
        }

        friend bool operator <=(const PoolIndex& a, const PoolIndex& b)
        {
            return a._pool_id <= b._pool_id && a._cell_id <= b._cell_id;
        }

        friend bool operator >=(const PoolIndex& a, const PoolIndex& b)
        {
            return a._pool_id >= b._pool_id && a._cell_id >= b._cell_id;
        }

        PoolIndex& operator =(const PoolIndex&) = default;

        PoolIndex& operator =(int32_t value)
        {
            _pool_id = (uint16_t)(value >> 16);
            _cell_id = (uint16_t)(value & 0xFFFF);
            return *this;
        }

        operator int32_t() const
        {
            return (((uint16_t)_pool_id) << 16) | _cell_id;
        }

        operator bool()const
        {
            return _pool_id != 65535 && _cell_id != 65535;
        }

        PoolIndex(int32_t value)
        :   _pool_id( 65535 )
        ,   _cell_id( 65535 )
        {
            operator =( value );
        }

        PoolIndex()
        :   _pool_id( 65535 )
        ,   _cell_id( 65535 )
        {}
        uint16_t _pool_id;
        uint16_t _cell_id;
    } PoolIndex;

//#else
//#define INVALID_INDEX uint32_t(-1)
//
//    typedef struct PoolIndex
//    {
//        friend bool operator ==(const PoolIndex& a, const PoolIndex& b)
//        {
//            return a._pool_id == b._pool_id && a._cell_id == b._cell_id;
//        };
//        friend bool operator !=(const PoolIndex& a, const PoolIndex& b)
//        {
//            return a._pool_id != b._pool_id || a._cell_id != b._cell_id;
//        };
//        friend bool operator <(const PoolIndex& a, const PoolIndex& b)
//        {
//            return int64_t(a) < int64_t(b);// a._pool_id <= b._pool_id && a._cell_id < b._cell_id;
//        };
//        friend bool operator >(const PoolIndex& a, const PoolIndex& b)
//        {
//            return int64_t(a) > int64_t(b);
//            //return a._pool_id >= b._pool_id && a._cell_id > b._cell_id;
//        };
//        friend bool operator <=(const PoolIndex& a, const PoolIndex& b)
//        {
//            return a._pool_id <= b._pool_id && a._cell_id <= b._cell_id;
//        };
//        friend bool operator >=(const PoolIndex& a, const PoolIndex& b)
//        {
//            return a._pool_id >= b._pool_id && a._cell_id >= b._cell_id;
//        };
//        PoolIndex& operator =(const PoolIndex&) = default;
//        PoolIndex& operator =(int64_t value)
//        {
//            _pool_id = (uint32_t)(value >> 32);
//            _cell_id = (uint32_t)(value & 0xFFFFFFFF);
//            return *this;
//        }
//        operator int64_t() const
//        {
//            return (((uint64_t)_pool_id) << 32) | _cell_id;
//        }
//        operator bool()const
//        {
//            return _pool_id != INVALID_INDEX && _cell_id != INVALID_INDEX;
//        }
//
//        PoolIndex()
//        :   _pool_id(-1)
//        ,   _cell_id(-1)
//        {}
//
//        uint32_t _pool_id = INVALID_INDEX;
//        uint32_t _cell_id = INVALID_INDEX;
//    } PoolIndex;
//
//#endif
}
#endif /* PoolIndex_hpp */
