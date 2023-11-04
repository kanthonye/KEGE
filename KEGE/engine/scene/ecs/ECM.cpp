//
//  ECM.cpp
//  CacheSystem
//
//  Created by Kenneth Esdaile on 3/14/23.
//

#include "ECM.hpp"
namespace kege
{
    ECM::ECM( uint32_t entity_pool_growsize, uint32_t max_distinct_pool_types, const std::vector< kege::PoolInfo >& pool_infos )
    :   _component_system( max_distinct_pool_types, pool_infos )
    ,   _entity_components({0, entity_pool_growsize})
    {}

    uint64_t ECM::bitmask(const EntityId& entity)const
    {
        return _entity_components.get( entity )->_bitmask._bitmask;
    }

    void ECM::destroy( const EntityId& entity )
    {
        _entity_components.get( entity )->clear();
        _entity_components.free( entity );
    }

    void ECM::clear( const EntityId& entity )
    {
        _entity_components.get( entity )->clear();
        //memset(_entity_components.get( entity )->_components, -1, sizeof(PoolIndex)*KEGE_MAX_COMPONENTS_PER_ENTITY);
    }

    EntityId ECM::create()
    {
        EntityId entity;
        _entity_components.allocate( &entity )->clear();
        return entity;
    }


    void ECM::purge()
    {
        _entity_components.purge();
    }

}
