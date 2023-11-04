//
//  ECS.cpp
//  PoolSysm
//
//  Created by Kenneth Esdaile on 3/11/23.
//

#include "ECS.hpp"
namespace kege{



    ECS::ECS( uint32_t entity_pool_growsize, uint32_t max_distinct_pool_types, const std::vector< kege::PoolInfo >& pool_infos )
    :   _ecm( entity_pool_growsize, max_distinct_pool_types, pool_infos )
    //,   _entity_systems()
    {}

    void ECS::unsubscribe( const uint32_t& event, kege::EntitySystem* system )
    {
        _eem.unsubscribe( event, system );
    }

    void ECS::subscribe( const uint32_t& event, kege::EntitySystem* system )
    {
        _eem.subscribe( event, system );
    }

    void ECS::post( const uint32_t& event, const EntityId& entity )
    {
        _eem.post( event, entity );
    }

    void ECS::operator()( const kege::Ref< Message >& message )
    {
        _esm.operator()( message );
    }

    kege::EntitySystem* ECS::getSystem( const kege::string& sid )
    {
        return _esm.get( sid );
    }

    void ECS::removeSystem( const kege::string& sid )
    {
        _esm.remove( sid );
    }

    void ECS::addSystem( kege::EntitySystem* system )
    {
        _esm.add( system );
    }

    void ECS::update( const double& dms )
    {
        _eem.broadcast();
        _esm.update( dms );
    }

    void ECS::render( const double& dms )
    {
        _esm.render( dms );
    }

    void ECS::uninit()
    {
        _esm.uninit();
    }

    bool ECS::init()
    {
        return _esm.init();
    }

    void ECS::integrate( const EntityId& entity )
    {
        _egm.integrate( entity, _ecm.bitmask( entity ) );
    }

    void ECS::segregate( const EntityId& entity )
    {
        _egm.segregate( entity, _ecm.bitmask( entity ) );
    }

    void ECS::destroy( const EntityId& entity )
    {
        _ecm.destroy( entity );
    }

    void ECS::clear(const EntityId& entity)
    {
        _ecm.clear( entity );
    }

    EntityId ECS::create()
    {
        return _ecm.create();
    }

    kege::EEM& ECS::eem()
    {
        return _eem;
    }

    kege::ESM& ECS::esm()
    {
        return _esm;
    }

    kege::EGM& ECS::egm()
    {
        return _egm;
    }

    kege::ECM& ECS::ecm()
    {
        return _ecm;
    }

    void ECS::purge()
    {
        _ecm.purge();
    }
}
