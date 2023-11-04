//
//  ECM.hpp
//  CacheSystem
//
//  Created by Kenneth Esdaile on 3/14/23.
//

#ifndef ECM_hpp
#define ECM_hpp
#include <set>
#include <map>
#include "ECO.hpp"
namespace kege
{
    /**
     * @brief ECM (Entity Component Manager) is a manager used to manage entities and their associated components.
     *
     * The ECM class is designed to manage entities and their associated components in an efficient manner.
     * It allows for the creation, retrieval, removal, and destruction of entities and components. The class
     * is initialized with parameters that define the size and type of component pools.
     *
     * @see kege::PoolInfo, kege::PoolMngrT, kege::PoolSysm, EntityId
     */
    class ECM
    {
    public:

        /**
         * @brief Constructor for ECM.
         *
         * Initializes the ECM with the specified parameters.
         *
         * @param entity_pool_growsize The initial size of the entity pool.
         * @param max_distinct_pool_types The maximum number of distinct component pool types.
         * @param pool_infos A vector of kege::PoolInfo objects describing component pool sizes and types.
         */
        ECM( uint32_t entity_pool_growsize, uint32_t max_distinct_pool_types, const std::vector< kege::PoolInfo >& pool_infos );

        /**
         * @brief Check if an entity has a specific component type associated with it.
         *
         * @tparam T The type of component to check.
         * @param entity The ID of the entity to check.
         * @return true if the entity has the specified component type; false otherwise.
         */
        template<typename T> bool has( const EntityId& entity )const;

        /**
         * @brief Remove a component from an entity.
         *
         * @tparam T The type of component to remove.
         * @param entity The ID of the entity to remove the component from.
         */
        template<typename T> void remove( const EntityId& entity );

        /**
         * @brief Add a new component to an existing entity.
         *
         * @tparam T The type of component to add.
         * @param entity The ID of the entity to add the component to.
         * @return A pointer to the added component.
         */
        template<typename T> T* add( const EntityId& entity );

        /**
         * @brief Retrieve a component from an entity.
         *
         * @tparam T The type of component to retrieve.
         * @param entity The ID of the entity to retrieve the component from.
         * @return A pointer to the retrieved component, or nullptr if not found.
         */
        template<typename T> T* get( const EntityId& entity );

        /**
         * @brief Get the component bitmask for an entity.
         *
         * @param entity The ID of the entity to get the bitmask for.
         * @return The component bitmask for the entity.
         */
        uint64_t bitmask(const EntityId& entity)const;

        /**
         * @brief Destroy an entity and its associated components.
         *
         * @param entity The ID of the entity to destroy.
         */
        void destroy(const EntityId& entity);

        /**
         * @brief Clear all components associated with an entity.
         *
         * @param entity The ID of the entity to clear components from.
         */
        void clear(const EntityId& entity);

        /**
         * @brief Create a new entity and return its unique identifier.
         *
         * @return The unique identifier of the newly created entity.
         */
        EntityId create();

        /**
         * @brief Delete all entities and components from the ECM.
         */
        void purge();

    private:

        /**
         * The _entity_components pool is responsible for managing the entities' components.
         */
        kege::PoolMngrT< ECO > _entity_components;

        /**
         * The _component_system pool is responsible for managing the storage of components.
         */
        kege::PoolSysm _component_system;
    };


    template<typename T> bool ECM::has( const EntityId& entity )const
    {
        if( !entity ) return false;
        const uint32_t COMP_TYPE = ECO::getComponetType< T >();
        return _entity_components.get( entity )->_bitmask.has( COMP_TYPE );
    }

    template<typename T> void ECM::remove( const EntityId& entity )
    {
        if( !entity ) return;
        const uint32_t COMP_TYPE = ECO::getComponetType< T >();

        ECO* eco = _entity_components.get( entity );
        //if( !components->has( COMP_TYPE ) ) return;
        _component_system.free( COMP_TYPE, eco->_components[ COMP_TYPE ] );
        eco->_bitmask.erase( COMP_TYPE );
    }

    template<typename T> T* ECM::add( const EntityId& entity )
    {
        if( !entity ) return nullptr;
        const uint32_t COMP_TYPE = ECO::getComponetType< T >();

        ECO* eco = _entity_components.get( entity );
        if( eco->has( COMP_TYPE ) )
        {
            return _component_system.get< T >( COMP_TYPE, eco->_components[ COMP_TYPE ] );
        }
        else
        {
            eco->set( COMP_TYPE );
            return _component_system.allocate< T >( COMP_TYPE, &eco->_components[ COMP_TYPE ] );
        }
    }

    template<typename T> T* ECM::get( const EntityId& entity )
    {
        if( !entity ) return nullptr;
        const uint32_t COMP_TYPE = ECO::getComponetType< T >();

        ECO* eco = _entity_components.get( entity );
        //if( !components->has( COMP_TYPE ) ) return nullptr;
        return _component_system.get< T >( COMP_TYPE, eco->_components[ COMP_TYPE ] );
    }
}

#endif /* ECM_hpp */
