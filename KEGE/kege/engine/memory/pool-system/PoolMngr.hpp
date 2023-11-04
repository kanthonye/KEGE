//
//  PoolMngr.hpp
//  testbed
//
//  Created by Kenneth Esdaile on 3/2/23.
//

#ifndef PoolMgr_hpp
#define PoolMgr_hpp
#include "Pool.hpp"
namespace kege
{
    /*
    The PoolInfo struct contains information about the size and layout of the memory pool.
    */
typedef struct PoolInfo
{
    uint32_t _type;
    uint32_t _size;
} PoolInfo;


    /**
    The PoolMngr class is intended to be subclassed to implement specific memory pool types with different allocation strategies.
    */
    class PoolMngr
    {
    public:

        /**
        `allocate( PoolIndex* access )`:
        is a pure virtual method that takes two pointers to uint32_t variables set and index, andcreturns a pointer to the allocated memory.
        This method is responsible for allocating memory from the pool and updating the set and index parameters to indicate the location
        of the allocated memory.
        */
        //virtual void* allocate( PoolIndex* access ) = 0;

        /**
        `get( const PoolIndex& access )`
        a pure virtual method that takes two uint32_t values set and index, and returns a pointer to the memory at the specified location
        in the pool.
        */
        //virtual void* get( const PoolIndex& access ) = 0;

        /**
        `free( const PoolIndex& access )`
        a pure virtual method that takes two uint32_t values set and index, and frees the memory at the specified location in the pool.
        */
        virtual void free( const PoolIndex& access ) = 0;

        /**
        `void uniteCellPool()`: a method that combines all sub-pools into a single large pool, which each sub-pool
        pointing to a different location in the unified pool memory.
        */
        virtual void uniteSubPools() = 0;

        virtual void purge() = 0;

        /**
        `PoolMngr(const PoolInfo& poolinfo)`:
        is  a constructor that takes a reference to a PoolInfo object and initializes the _poolinfo member variable.
         */
        PoolMngr( const PoolInfo& poolinfo ): _poolinfo( poolinfo ) {}

        /**
        ~PoolMngr(): a virtual destructor that does nothing.
         */
        virtual ~PoolMngr(){}

        PoolInfo _poolinfo;
    };



    /**
    This code defines a class PoolMngrT that manages a pool of PoolCell objects, where each PoolCell contains a piece of data of type Data. The PoolMngrT
    class is similar to the Pool class, but it manages multiple Pool objects and can optionally unify them into a single pool.
    */
    template< typename Data > class PoolMngrT : public PoolMngr
    {
    public:

        /**
        `bool isInList(const Pool< Data >* pool)`: a method that takes a pointer to a Pool object pool
        and returns a boolean value indicating whether the Pool object is already in the list of sub-pools managed by the PoolMngrT object.
        */
        bool isInList( const Pool< Data >* pool );

        /**
        `Data* allocate( PoolIndex* access )`:
        is a method that takes a pointers to a PoolIndex variable, set the pool-id which is the index of the
        pool that contains the data that the cell-id will belong to. Cell-id is the pool index where the data is
        located. Once the pool-id and cell-id is set, a pointer to the pool data is returned
        */
        Data* allocate( PoolIndex* access );

        /**
        `void* get( const PoolIndex& access )`:
        is a method that takes a PoolIndex, and returns a pointer to the piece of data of type Data at the
        specified index in the sub-pool.
        */
        Data* get( const PoolIndex& access );
        const Data* get( const PoolIndex& access )const;

        /**
        `void free( const PoolIndex& access )`: a method that takes two uint32_t values set and index
        and frees the PoolCell at the specified index in the sub-pool
        */
        void free( const PoolIndex& access );

        /**
        `void uniteCellPool()`: a method that combines all sub-pools into a single large pool, which each sub-pool
        pointing to a different location in the unified pool memory.
        */
        void uniteSubPools();

        void purge();

        PoolMngrT( const PoolInfo& poolinfo );
        ~PoolMngrT();

        std::vector< Pool< Data >* > _pools;
        /**
        `_pools`: is all the creaed pools manages by this object
        */

        PoolCell< Data >* _unified_pool;
        /**
        `_unified_pool`: If initializes, it combines all `Pool` object managed by this instace into one large pool,
        which each`Pool` pointer in the `_pools`vector points to its corresponding index location in the allocated
        `_unified_pool` memory. Each index is offset by `_poolinfo._size`. Note that newly added `Pool`
        will not have an index into the `_unified_pool` memory, unless `uniteCellPools()` is called once more.
        */

        Pool< Data >* _head;
        /**
        `_head`: is the head of a linked list of `Pool` objects that have free avaiable `Cells` ready to be reused
        */
    };

    template< typename Data > bool PoolMngrT< Data >::isInList( const Pool< Data >* pool )
    {
        return pool == _head || pool->_prev_pool != nullptr;
    }

    template< typename Data > Data* PoolMngrT< Data >::allocate( PoolIndex* access )
    {
        if ( _head )
        {
            access->_pool_id = _head->_id;
            Data* data = _head->allocate( access );
            if ( _head->full() )
            {
                _head->_prev_pool = nullptr;
                _head = _head->_next_pool;
            }
            return data;
        }

        Pool< Data >* pool = new Pool< Data >
        (
            new PoolCell< Data >[ _poolinfo._size ],
            _poolinfo._size,
            true
        );

        if ( _head == nullptr )
        {
            _head = pool;
        }
        else
        {
            _head->_prev_pool = pool;
            pool->_next_pool = _head;
            _head = pool;
        }

        _pools.push_back( pool );

        pool->_id = uint32_t( _pools.size() - 1 );
        access->_pool_id = pool->_id;

        return _pools[ access->_pool_id ]->allocate( access );
    }
    template< typename Data > Data* PoolMngrT< Data >::get( const PoolIndex& access )
    {
        if( !access ) return nullptr;
        return _pools[ access._pool_id ]->get( access );
    }
    template< typename Data > const Data* PoolMngrT< Data >::get( const PoolIndex& access )const
    {
        if( !access ) return nullptr;
        return _pools[ access._pool_id ]->get( access );
    }
    template< typename Data > void PoolMngrT< Data >::free( const PoolIndex& access )
    {
        _pools[ access._pool_id ]->free( access );
        if( !isInList( _pools[ access._pool_id ] ) )
        {
            _pools[ access._pool_id ]->_next_pool = _head;
            _head = _pools[ access._pool_id ];
        }
    }
    template< typename Data > void PoolMngrT< Data >::uniteSubPools()
    {
        size_t size = _pools.size() * _poolinfo._size;
        PoolCell< Data >* unified_pool = new PoolCell< Data >[ size ];
        size_t block_size_in_bytes = sizeof( PoolCell< Data > ) * _poolinfo._size;

        Pool< Data >* pool;
        PoolCell< Data >* cells;
        PoolCell< Data >* pool_cell;
        size_t offset_head, offset_curr;
        size_t offset_next;

        for (int i=0; i<_pools.size(); i++)
        {
            pool = _pools[ i ];
            cells = &unified_pool[ _poolinfo._size * i ];
            memcpy( cells, pool->_cells, block_size_in_bytes );

            offset_head = pool->_free_head - (&pool->_cells[ 0 ]);

            while ( pool->_free_head != nullptr )
            {
                offset_curr = pool->_free_head - (&pool->_cells[ 0 ]);
                pool_cell = &pool->_cells[ offset_curr ];

                if( pool_cell->_next == nullptr )
                {
                    cells[ offset_curr ]._next = nullptr;
                    break;
                }

                offset_next = pool_cell->_next - (&pool->_cells[0]);
                cells[ offset_curr ]._next = &cells[ offset_next ];
                pool->_free_head = pool->_free_head->_next;
            }

            pool->purge();
            pool->_cells = cells;
            pool->_allocated = false;
            if ( pool->_free_head != nullptr )
            {
                pool->_free_head = &cells[ offset_head ];
            }
        }
        _unified_pool = unified_pool;
    }

    template< typename Data > void PoolMngrT< Data >::purge()
    {
        for (int i = 0; i < _pools.size(); i++)
        {
            delete _pools[i];
        }
        _pools.clear();

        if ( _unified_pool )
        {
            delete [] _unified_pool;
            _unified_pool = nullptr;
        }
        _head = nullptr;
    }

    template< typename Data > PoolMngrT< Data >::PoolMngrT( const PoolInfo& poolinfo )
    :   PoolMngr( poolinfo )
    ,	_unified_pool( nullptr )
    ,   _head( nullptr )
    {}
    template< typename Data > PoolMngrT< Data >::~PoolMngrT()
    {
        purge();
    }

}
#endif /* PoolMgr_hpp */
