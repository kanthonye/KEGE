//
//  MemPool.cpp
//  testbed
//
//  Created by Kenneth Esdaile on 2/20/23.
//

#include "PoolSysm.hpp"

namespace kege
{

    PoolSysm::PoolSysm(uint32_t max_distinct_pool_types, const std::vector< PoolInfo >& pool_infos)
    :   _pool_infos()
    {
        for (int i=0; i<pool_infos.size(); i++)
        {
            if ( _pool_infos.find( pool_infos[i]._type ) == _pool_infos.end() )
            {
                _pool_infos[ pool_infos[i]._type ] = pool_infos[i]._size;
            }
            else
            {
                throw std::runtime_error( "dupiclate PoolInfo type is not allowed." );
            }
        }

        if ( max_distinct_pool_types < _pool_infos.size() )
        {
            throw std::runtime_error( "[error] PoolSysm(max_distinct_pool_types, pool_infos): params -> pool_infos size has to be larger than or equal param -> max_distinct_pool_types" );
        }
        _poollist.resize( max_distinct_pool_types );
        memset( _poollist.data(), 0x0, max_distinct_pool_types * 8 );
    }

    void PoolSysm::free( uint32_t type, const PoolIndex& access )
    {
        _poollist[ type ]->free( access );
    }

    void PoolSysm::purge()
    {
        _pool_infos.clear();
        for (int i=0; i<_poollist.size(); i++)
        {
            if ( _poollist[ i ] != nullptr )
            {
                delete _poollist[ i ];
                _poollist[ i ] = nullptr;
            }
        }
        _poollist.clear();
    }

    PoolSysm::~PoolSysm()
    {
        purge();
    }

}
