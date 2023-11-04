//
//  EntitySystemManager.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/1/23.
//

#ifndef EntitySystemManager_hpp
#define EntitySystemManager_hpp

#include "Hashmap.hpp"
#include "../ecs/EntitySystem.hpp"
#include "../ecs/EntitySetManager.hpp"

namespace kege{

    /**
     * @brief EntitySystemManager is a manager class that organizes and manages kege::System objects.
     *
     * The EntitySystemContainer class provides a container for managing kege::System objects, such as
     * those used in an entity-component-system (ECS) architecture. It allows adding, removing, updating,
     * and rendering systems. Systems are organized by their unique IDs and can be accessed and manipulated
     * through this container.
     *
     * @see kege::System, kege::ECS, kege::Message
     */
    typedef class EntitySystemManager
    {
    public:

        /**
         * @brief Add a system to the manager.
         * @param system A pointer to the system to add.
         */
        void add( const kege::Ref< kege::EntitySystem >& system );

        /**
         * @brief Forward a message to the systems.
         * @param message A reference to the message to forward.
         */
        void operator()( const kege::Ref< Message >& message );

        /**
         * @brief Get a system with a specific ID.
         * @param sid The ID of the system to retrieve.
         * @return A pointer to the system with the specified ID, or nullptr if not found.
         */
        kege::EntitySystem* get( const kege::string& sid );

        /**
         * @brief Remove a system with a specific ID.
         * @param sid The ID of the system to remove.
         */
        void remove( const kege::string& sid );

        /**
         * @brief Update the systems.
         * @param dms The time in milliseconds since the last update.
         */
        void update( const double& dms );

        /**
         * @brief Render the systems.
         * @param dms The time in milliseconds since the last render.
         */
        void render( const double& dms );

        /**
         * @brief Uninitialize the EntitySystemContainer.
         */
        void uninit();

        /**
         * @brief Initialize the EntitySystemContainer.
         * @return true if initialization was successful; false otherwise.
         */
        bool init();

        /**
         * @brief Delete all systems from the container.
         */
        void purge();

        /**
         * @brief Destructor for EntitySystemContainer.
         */
        ~EntitySystemManager();

        /**
         * @brief Constructor for EntitySystemContainer.
         */
        EntitySystemManager();

    private:

        std::map< uint32_t, std::vector< kege::EntitySystem* > > _systems;
        kege::Hashmap< kege::Ref< kege::EntitySystem > > _hashmap;

    } ESM;

}

#endif /* EntitySystemManager_hpp */
