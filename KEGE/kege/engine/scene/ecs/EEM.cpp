//
//  EEM.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/24/23.
//

#include "EEM.hpp"
namespace kege
{
    void EEM::unsubscribe( const uint32_t& event, kege::EntitySystem* system )
    {
        if ( _listeners.find( event ) != _listeners.end() )
        {
            std::vector< kege::EntitySystem* >* v = _listeners[ event ];
            for ( auto i = v->begin(); i !=  v->end(); i++ )
            {
                if ( (*i) == system )
                {
                    v->erase(i);
                    break;
                }
            }
        }
    }

    void EEM::subscribe( const uint32_t& event, kege::EntitySystem* system )
    {
        if ( _listeners.find( event ) == _listeners.end() )
        {
            std::vector< kege::EntitySystem* >* v = new std::vector< kege::EntitySystem* >;
            _listeners[ event ] = v;
            v->push_back( system );
        }
        else
        {
            _listeners[ event ]->push_back( system );
        }
    }

    void EEM::dispatch( const uint32_t& event, const EntityId& entity )
    {
        auto i = _listeners.find( event );
        if ( i == _listeners.end() ) return;

        for ( auto& sys : *i->second )
        {
            if ( sys == nullptr ) continue;
            if ( sys->has( entity ) )
            {
                sys->operator()( event, entity );
            }
        }
    }

    void EEM::post( const uint32_t& event, const EntityId& entity )
    {
        std::lock_guard< std::mutex > lock( _mutex_messages );
        _events.push_back({ event, entity });
    }

    void EEM::broadcast()
    {
        for ( const std::pair< uint32_t, EntityId >& event: _events )
        {
            dispatch( event.first, event.second );
        }
        _events.clear();
    }

    void EEM::purge()
    {
        for ( auto& m: _listeners )
        {
            delete m.second;
        }
        _listeners.clear();
        _events.clear();
    }

    EEM::~EEM()
    {
        purge();
    }

    EEM::EEM()
    {}


}
