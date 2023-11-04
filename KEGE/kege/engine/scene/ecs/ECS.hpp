//
//  ECS.hpp
//  PoolSysm
//
//  Created by Kenneth Esdaile on 3/11/23.
//

#ifndef ECS_hpp
#define ECS_hpp
#include "ECM.hpp"
#include "EEM.hpp"
#include "EntitySetManager.hpp"
#include "EntitySystemManager.hpp"
namespace kege
{
    class ECS
    {
    public:

        ECS( uint32_t entity_pool_growsize, uint32_t max_distinct_pool_types, const std::vector< kege::PoolInfo >& pool_infos );

        std::set< EntityId >* getEntitySet( uint64_t component_bitmask );

        /**
        `unsubscribe(const uint32_t& event, kege::System* system):`
        Remove a system from the listeners of a specific event.
        */
        void unsubscribe( const uint32_t& event, kege::EntitySystem* system );

        /**
        `subscribe(const uint32_t& event, kege::System* system):`
        Add a system to the listeners of a specific event.
        */
        void subscribe( const uint32_t& event, kege::EntitySystem* system );

        /**
        `post(const uint32_t& event, const EntityId& entity):`
        Post an event for a specific entity.
        */
        void post( const uint32_t& event, const EntityId& entity );

        /**
        `operator()( const Message& message )`
        functions is used to forward messages to the systems
         */
        void operator()( const kege::Ref< Message >& message );

        /**
        `get(const kege::string& sid):`
        Get a system with a specific id.
        */
        kege::EntitySystem* getSystem( const kege::string& sid );

        /**
        `remove(const kege::string& sid):`
        Remove a system with a specific id.
        */
        void removeSystem( const kege::string& sid );

        /**
        `add(kege::System* system):`
        Add a system to the manager.
        */
        void addSystem( kege::EntitySystem* system );

        /**
        `update(const double& dms):`
         Update the systems.
        */
        void update( const double& dms );

        /**
        `render( const double& dms ):`
        Render the systems.
        */
        void render( const double& dms );

        /**
        Uninitialize the ESM.
        */
        void uninit();

        /**
        Initialize the ESM
        */
        bool init();


        /**
        The `add()` function is used to add a new component to an existing entity
        */
        template<typename T> T* addComponent( const EntityId& entity, const T& component );
        template<typename T> T* addComponent( const EntityId& entity );

        /**
        The `has()` function can be used to check if an entity has a specific component type associated with it
        */
        template<typename T> bool hasComponent( const EntityId& entity )const;

        /**
        the `erase()` function removes a component from an entity
        */
        template<typename T> void eraseComponent( const EntityId& entity );

        /**
        The `get()` function is used to retrieve a component from an entity
        */
        template<typename T> T* getComponent( const EntityId& entity );

        /**
        The `integrate()` functions is used to add entities to the systems managed by the `ESM`
         */
        void integrate( const EntityId& entity );

        /**
        The `segregate()` functions is used to remove entities from the systems managed by the `ESM`
         */
        void segregate( const EntityId& entity );

        /**
        The `destroy()` function removes an entity and its associated components from the ECS system
        */
        void destroy(const EntityId& entity);

        /**
        The `clear()` function removes all components associated with the EntityId entity
        */
        bool empty(const EntityId& entity)const;

        /**
        The `clear()` function removes all components associated with the EntityId entity
        */
        void clear(const EntityId& entity);

        /**
        The `create()` function is used to create a new entity and returns the unique identifier for the new entity
        */
        EntityId create();

        kege::EEM& eem();
        kege::ESM& esm();
        kege::ECM& ecm();
        kege::EGM& egm();

        void purge();
        
    private:

        /**
         * ECM (Entity Component Manager) manages components and entities
         */
        kege::ECM _ecm;

        /**
         * ESM (Entity Group Manager) manages entity sets. entities are grouped into sets by system signature
         */
        kege::EGM _egm;

        /**
         * ESM (Entity Systems Manager), manages entity systems
         */
        kege::ESM _esm;

        /**
         * EEM (Entity Event Manager) manages entity events
         */
        kege::EEM _eem;
    };

}



namespace kege
{

    template<typename T> bool ECS::hasComponent( const EntityId& entity )const
    {
        return _ecm.has< T >( entity );
    }

    template<typename T> void ECS::eraseComponent( const EntityId& entity )
    {
        _ecm.remove< T >( entity );
    }

    template<typename T> T* ECS::addComponent( const EntityId& entity, const T& component )
    {
        T* t = _ecm.add< T >( entity );
        *t = component;
        return t;
    }
    template<typename T> T* ECS::addComponent( const EntityId& entity )
    {
        return _ecm.add< T >( entity );
    }

    template<typename T> T* ECS::getComponent( const EntityId& entity )
    {
        return _ecm.get< T >( entity );
    }

}

#endif /* ECS_hpp */
