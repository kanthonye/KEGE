//
//  EntitySetManager.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/1/23.
//

#include "EntitySetManager.hpp"
namespace kege{

    void EntitySetManager::integrate( const EntityId& entity, uint64_t component_bitmask )
    {
        // insert the entity into all the sets that share the same components.
        for ( auto& i : _sets )
        {
            if ( (i.first & component_bitmask) != i.first ) continue;
            i.second->insert( entity );
        }
    }

    void EntitySetManager::segregate( const EntityId& entity, uint64_t component_bitmask )
    {
        // remove the entity from all the sets that share the same components.
        for ( auto& i : _sets )
        {
            if ( (i.first & component_bitmask) != component_bitmask ) continue;
            i.second->erase( entity );
        }
    }

    std::set< EntityId >* EntitySetManager::get( uint64_t component_bitmask )
    {
        if( component_bitmask != 0L )
        {
            std::set< EntityId >* entity_set = nullptr;
            auto i = _sets.find( component_bitmask );
            if( i == _sets.end() )
            {
                entity_set = new std::set< EntityId >;
                _sets[ component_bitmask ] = entity_set;
            }
            else entity_set = i->second;
            return entity_set;
        }
        return nullptr;
    }

    void EntitySetManager::clear()
    {
        for ( auto& i : _sets )
        {
            delete i.second;
        }
        _sets.clear();
    }

}
