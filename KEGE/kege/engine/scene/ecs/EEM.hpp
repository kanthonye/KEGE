//
//  EEM.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/24/23.
//

#ifndef KEGE_EEM
#define KEGE_EEM

#include <stdio.h>
#include "ECM.hpp"
#include "../ecs/EntitySystem.hpp"
namespace kege
{
    /**
     * @brief EEM (Entity Event Manager) coordinates the events that happens to entities.
     * Systems are then notifies of these events. If a system contains the entity then it
     * is allowed to handle the event, otherwise the system will not be notified.
     *
     * The EEM is an object that is managed by the ECS object.
     *
     * @note Systems that want to handle any events for the entites they contin can
     * subscribe for any events its interested in.
     *
     * @see kege::ECS, core::Buffer
     */
    class EEM
    {
    public:

        /**
         * @brief Unsubscribe a system from a specific event channed.
         * @param event The index of the event channel which the system is located.
         * @param system The system pointer to remove from the event channel
         */
        void unsubscribe( const uint32_t& event, kege::EntitySystem* system );

        /**
         * @brief Subscribe a system to a specific event channed.
         * @param event The index where the channel is located.
         * @param system The system to add to the event channel
         */
        void subscribe( const uint32_t& event, kege::EntitySystem* system );

        /**
         * @brief notify systems of an entity event
         * @param event The enum event-id
         * @param entity The entity-id of the entity the event is generated for
         */
        void dispatch( const uint32_t& event, const EntityId& entity );

        /**
         * @brief post() store the event for later dispatching
         * @param event The enum event-id
         * @param entity The EntityId of the entity which the event is generated for
         */
        void post( const uint32_t& event, const EntityId& entity );

        /**
         * @brief Dispatch all posted events
         */
        void broadcast();

        /**
         * @brief release all allocated memeory. perform cleanup.
         */
        void purge();

        ~EEM();
        EEM();

    private:

        std::unordered_map< uint32_t, std::vector< kege::EntitySystem* >* > _listeners;
        std::vector< std::pair< uint32_t, EntityId > > _events;
        std::mutex _mutex_messages;
    };

}
#endif /* KEGE_EEM */
