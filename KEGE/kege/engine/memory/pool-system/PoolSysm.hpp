//
//  PoolSysm.hpp
//  testbed
//
//  Created by Kenneth Esdaile on 2/20/23.
//

#ifndef PoolSystem_hpp
#define PoolSystem_hpp
#include "PoolMngr.hpp"
namespace kege
{

    /**
    The  class `PoolSysm` manages a set of PoolMngr objects, where each PoolMngr object manages a pool of objects of a specific type.
    */
    class PoolSysm
    {
    public:

        /**
        `PoolSysm(uint32_t max_distinct_pool_types, const std::vector< PoolInfo >& pool_infos)`:
        The constructor that takes a maximum number of distinct pool types max_distinct_pool_types and a vector of PoolInfo objects
        pool_infos, which describe the parameters of each pool. The constructor initializes the _poollist vector with the appropriate
        number of PoolMngr objects and sets up the mapping between pool types and their indices in the _poollist vector.
        */
        PoolSysm(uint32_t max_distinct_pool_types, const std::vector< PoolInfo >& pool_infos);

        /**
        `template<typename T> T* get(uint32_t type, const PoolIndex& access)`: a method that takes an PoolIndex
         object access and returns a void pointer to the object at the specified location in the corresponding pool. The method looks
         up the index of the pool type associated with the object, then calls the get method of the corresponding PoolMngr object to
         get the object.
        */
        template<typename T> T* get(  uint32_t type, const PoolIndex& access );
        template<typename T> const T* get(  uint32_t type, const PoolIndex& access )const;

        /**
        `template<typename T> T* allocate(uint32_t type, PoolIndex* access)`: a method that takes a type index and
        pointer to an PoolIndex object access and returns a pointer to a newly allocated object of type T. The method first looks up the index
        of the pool type associated with T. Then calls the allocate method of the corresponding PoolMngr object to allocate the object.
        */
        template<typename T> T* allocate( uint32_t type, PoolIndex* access );

        /**
        `void free(uint32_t type, const PoolIndex& access)`: a method that takes an PoolIndex object access and frees the object at the specified
        location in the corresponding pool. The method looks up the index of the pool type associated with the object.  then calls the free
        method of the corresponding PoolMngr object to free the object.
        */
        void free(  uint32_t type, const PoolIndex& access );

        void purge();
        ~PoolSysm();

    private:

        /**
        `_poollist`: a vector of pointers to PoolMngr objects, each of which manages a pool of objects of a specific type
        */
        std::vector< PoolMngr* > _poollist;

        /**
        `_pool_infos`: an unordered map that maps pool type indices to their corresponding pool size.
        */
        std::unordered_map< uint32_t,uint32_t > _pool_infos;
    };

    template<typename T> T* PoolSysm::allocate( uint32_t type, PoolIndex* access )
    {
        if ( type >= _poollist.size() ) return nullptr;
        if ( _poollist[ type ] == nullptr )
        {
            uint32_t poolsize = _pool_infos[ type ];
            _poollist[ type ] = new PoolMngrT< T >( { type, poolsize } );
        }
        if ( _poollist[ type ]->_poolinfo._type != type )
        {
            throw std::runtime_error( "[error] PoolSysm::allocate(access): access type and pool type mismatch" );
        }
        return ((PoolMngrT< T >*) _poollist[ type ])->allocate( access );
    }

    template<typename T> T* PoolSysm::get( uint32_t type, const PoolIndex& access )
    {
        if ( type >= _poollist.size() ) return nullptr;
        return ((PoolMngrT< T >*) _poollist[ type ])->get( access );
    }
    
    template<typename T> const T* PoolSysm::get( uint32_t type, const PoolIndex& access )const
    {
        if ( type >= _poollist.size() ) return nullptr;
        return ((PoolMngrT< T >*) _poollist[ type ])->get( access );
    }

}


#endif /* PoolSystem_hpp */
