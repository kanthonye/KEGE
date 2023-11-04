//
//  EntitySetManager.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/1/23.
//

#ifndef EntitySetManager_hpp
#define EntitySetManager_hpp

#include <set>
#include <map>
#include <stdlib.h>
#include "../ecs/ECO.hpp"

namespace kege{

    /**
     * @brief EntitySetManager (EGM) groups entities based on component signatures.
     *
     * The EntitySetManager (EGM) is responsible for organizing entities into sets based on their component signatures.
     * It provides functions to integrate entities into sets and segregate entities from sets, making it easier to
     * query and manage entities with specific combinations of components.
     *
     * @see EntityId
     */
    typedef class EntitySetManager
    {
    public:

        /**
         * @brief Integrate an entity into a set based on its component bitmask.
         *
         * @param entity The ID of the entity to integrate into the set.
         * @param component_bitmask The bitmask representing the entity's component signature.
         */
        void integrate( const EntityId& entity, uint64_t component_bitmask );

        /**
         * @brief Segregate an entity from a set based on its component bitmask.
         *
         * @param entity The ID of the entity to segregate from the set.
         * @param component_bitmask The bitmask representing the entity's component signature.
         */
        void segregate( const EntityId& entity, uint64_t component_bitmask );

        /**
         * @brief Get a set of entities based on a specific component bitmask.
         *
         * @param component_bitmask The bitmask representing the desired component signature.
         * @return A pointer to the set of entities matching the specified component bitmask.
         */
        std::set< EntityId >* get( uint64_t component_bitmask );

        /**
         * @brief Remove a set of entities based on a specific component bitmask.
         *
         * @param component_bitmask The bitmask representing the component signature of the set to remove.
         */
        void remove( uint64_t component_bitmask );

        /**
         * @brief Clear all sets and remove all entities from the EntitySetManager.
         */
        void clear();

    private:

        std::map< uint64_t, std::set< EntityId >* > _sets;
    } EGM;

}
#endif /* EntitySetManager_hpp */
