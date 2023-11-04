//
//  ECO.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/23/23.
//

#ifndef ECO_hpp
#define ECO_hpp
#include <stdlib.h>
#include "TypeId.hpp"
#include "PoolSysm.hpp"
#define KEGE_MAX_COMPONENTS_PER_ENTITY 32

namespace kege{

    class Bitmask
    {
    public:

        bool has( uint32_t type )const;

        void erase( uint32_t type );

        void set( uint32_t type );

        void clear();
        Bitmask();

        uint64_t _bitmask;
    };
}




namespace kege{

    class Message;
    typedef kege::PoolIndex EntityId;

    /**
     `ECO (Entity Component Object)`  is used by ECM to store the components for an entity.
     */
    class ECO
    {
    public:

        template<typename T> static uint32_t getComponetType();

        bool has( uint32_t type )const;

        void erase( uint32_t type );

        void set( uint32_t type );

        void clear();
        
        /**
        `_bitmask` is a 64-bit integer that is used to store the state of the collection. Each bit in the _bitmask variable corresponds to a specific component type.
        */
        Bitmask _bitmask;

        /**
        `components` is an array of kege::PoolIndex objects that contains the indices of the components in a memory pool.
        The maximum number of components per entity is defined by MAX_COMPONENTS_PER_ENTITY.
         */
        kege::PoolIndex _components[ KEGE_MAX_COMPONENTS_PER_ENTITY ];
    };

    template<typename T> uint32_t ECO::getComponetType()
    {
        return kege::TypeId< ECO, T >::getEnumTypeId();
    }
}
#endif /* ECO_hpp */
