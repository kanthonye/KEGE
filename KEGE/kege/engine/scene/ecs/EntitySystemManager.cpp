//
//  EntitySystemManager.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/1/23.
//

#include "EntitySystemManager.hpp"
namespace kege{

    void EntitySystemManager::add( const kege::Ref< kege::EntitySystem >& sys )
    {
        kege::Ref< kege::EntitySystem > s = sys;
        _hashmap.insert( sys->name(), s );
        _systems[ sys->priority() ].push_back( s.ref() );
    }

    void EntitySystemManager::operator()( const kege::Ref< Message >& message )
    {
        for ( auto& i : _systems )
        {
            for ( auto& sys : i.second )
            {
                sys->operator()( message );
            }
        }
    }

    kege::EntitySystem* EntitySystemManager::get( const kege::string& sid )
    {
        return _hashmap.get( sid ).ref();
    }

    void EntitySystemManager::remove( const kege::string& sid )
    {
        kege::EntitySystem* system = _hashmap.get( sid ).ref();
        if ( system == nullptr) return;
        _hashmap.remove( sid );

        std::vector< kege::EntitySystem* >& systems = _systems[ system->priority() ];
        auto m = std::find(systems.begin(), systems.end(), system);
        if ( m != systems.end() )
        {
            systems.erase( m );
        }
    }

    void EntitySystemManager::update( const double& dms)
    {
        // update systems
        for ( auto& i : _systems )
        {
            for ( auto& sys : i.second )
            {
                sys->update( dms );
            }
        }
    }

    void EntitySystemManager::render( const double& dms )
    {
        // render systems
        for ( auto& i : _systems )
        {
            for ( auto& sys : i.second )
            {
                sys->render( dms );
            }
        }
    }

    void EntitySystemManager::uninit()
    {
        for ( auto i = _systems.rbegin(); i != _systems.rend(); i++ )
        {
            for ( auto& sys : i->second )
            {
                sys->uninit();
            }
        }
    }

    bool EntitySystemManager::init()
    {
        for ( auto i = _systems.rbegin(); i != _systems.rend(); i++ )
        {
            for ( auto& sys : i->second )
            {
                if( !sys->init() )
                {
                    return false;
                }
            }
        }
        return true;
    }

    void EntitySystemManager::purge()
    {
        uninit();
        _hashmap.clear();
        _systems.clear();
    }

    EntitySystemManager::~EntitySystemManager()
    {
        purge();
    }

    EntitySystemManager::EntitySystemManager()
    {
    }
}
