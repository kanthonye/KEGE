//
//  EntitySystem.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/1/23.
//

#include "Entity.hpp"
#include "EntitySystem.hpp"
namespace kege
{
    void EntitySystem::setEntitySet( std::set< EntityId >* entities )
    {
        _entities = entities;
    }
    
    bool EntitySystem::has( const EntityId& entity )const
    {
        return false;
    }

    bool EntitySystem::init()
    {
        if ( _components._bitmask )
        {
            _entities = Entity::egm()->get( _components._bitmask );
        }
        return true;
    }

    uint64_t EntitySystem::componentBitmask()const
    {
        return _components._bitmask;
    }
    
    uint32_t EntitySystem::priority()const
    {
        return _priority;
    }

    EntitySystem::EntitySystem( uint32_t priority, const kege::string& name  )
    :   kege::System( name )
    ,   _priority(priority)
    ,   _entities( nullptr )
    ,   _init( false )
    ,   _components()
    {
        _components._bitmask = 0;
    }

    EntitySystem:: ~EntitySystem()
    {}

};
