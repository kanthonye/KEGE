//
//  Entity.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/24/23.
//

#ifndef Entity_hpp
#define Entity_hpp

//#include "kege-string.hpp"
#include "transform.hpp"
#include "../ecs/ECS.hpp"

namespace kege{namespace component{

    struct Children
    {
        kege::PoolIndex _head = KEGE_INVALID_POOL_INDEX;
        kege::PoolIndex _tail = KEGE_INVALID_POOL_INDEX;
        uint32_t _count = 0;
    };

    struct Child
    {
        kege::PoolIndex _parent = KEGE_INVALID_POOL_INDEX;
        kege::PoolIndex _entity = KEGE_INVALID_POOL_INDEX;
        kege::PoolIndex _next   = KEGE_INVALID_POOL_INDEX;
        kege::PoolIndex _prev   = KEGE_INVALID_POOL_INDEX;
        kege::transform _transform = {};
    };

    typedef kege::string Name;

}}

namespace kege
{
    struct EntityInitInfo
    {
        uint32_t _entity_pool_growsize;
        uint32_t _max_distinct_pool_types;
        std::vector< kege::PoolInfo > _pool_infos;
    };

    class Entity
    {
    public:

        /**
         get entity
         @param name is the name of the entity to remove.
         @return return a child entity if exit.
         */
        kege::Entity getChild( const kege::string& name );

        /**
         add child entity to the back of children list.
         @param entity is the child to attach to parent.
         @return return added child entity.
         */
        kege::Entity& prepend( kege::Entity& entity );

        /**
         add child entity to the front of children list.
         @param entity is the child to attach to parent.
         @return return added child entity.
         */
        kege::Entity& append( kege::Entity& entity );

        /**
         get parent entity of this child entity.
         @return return parent entity.
         */
        kege::Entity getParent()const;

        /**
         get root entity of this child entity.
         @return return a root entity.
         */
        kege::Entity getRoot()const;

        /**
         check if this entity is a parent entity.
         @return true if entity is parent, false otherwise.
         */
        bool isParent()const;

        /**
         check if this entity is a child entity.
         @return true if entity is a child, false otherwise.
         */
        bool isChild()const;

        /**
         remove the first child in entity child list.
         */
        void detachHead();

        /**
         remove the last child in entity child list.
         */
        void detachTail();

        /**
         remove all children in entity child list.
         */
        void detachAll();

        /**
         remove self from parent entity child list.
         */
        void detach();

        /**
        The `add()` function is used to add a new component to an existing entity
        */
        template<typename T> T* add( const T& component );
        template<typename T> T* add();

        /**
        The `has()` function can be used to check if an entity has a specific component associated with it
        */
        template<typename T> bool has()const;

        /**
        the `erase()` function removes a component from this entity
        */
        template<typename T> void erase();

        /**
        The `get()` function is used to retrieve a component data from an entity
        */
        template<typename T> T* get();

        /**
        The `integrate()` functions is used to make an entity visible to the systems that fits its signature
         */
        void integrate( EntityId eid );
        void integrate();

        /**
        The `segregate()` functions is used to remove entities from the systems managed by the `ECS`
         */
        void segregate( EntityId eid );
        void segregate();

        /**
        The `destroy()` function removes all the components and this entity from the ECS
        */
        void destroy();

        /**
        The `purge()` function removes all components associated with this
        */
        void purge();

        operator bool()const;

        static component::Child* head( component::Children* component );
        static component::Child* tail( component::Children* component );
        static component::Child* next( component::Child* component );
        static component::Child* prev( component::Child* component );

        static bool init( const EntityInitInfo& info );
        static kege::Entity create();
        static kege::ESM* systems();
        static kege::EGM* egm();
        static kege::ECM* ecm();
        static void uninit();

        //bool operator==( const Entity& e1, const Entity& e2 );
        Entity& operator=( const EntityId& id );
        EntityId id()const;

        Entity( const Entity& entity );
        Entity( const EntityId& id );
        Entity();

    private:

        void destroy( EntityId entity_id );

        /**
         The `ESM (Entity Systems Manager)` is responsible for managing the systems that process the entities in the ECS system
         */
        static kege::ECS* _ecs;

        /*
         The Entity Identifier to minipulate the components with in the ECS.
         */
        EntityId _entity_id;
    };



    inline bool operator ==( const Entity& e1, const Entity& e2 )
    {
        return e1.id() == e2.id();
    }

    inline bool operator !=( const Entity& e1, const Entity& e2 )
    {
        return e1.id() != e2.id();
    }

    template<typename T> T* Entity::add( const T& component )
    {
        T* comp = this->add< T >();
        *comp = component;
        return comp;
    }
    
    template<typename T> T* Entity::add()
    {
        return _ecs->addComponent< T >( _entity_id );
    }

    template<typename T> bool Entity::has()const
    {
        return _ecs->hasComponent< T >( _entity_id );
    }

    template<typename T> void Entity::erase()
    {
        _ecs->eraseComponent< T >( _entity_id );
    }

    template<typename T> T* Entity::get()
    {
        return _ecs->getComponent< T >( _entity_id );
    }

    enum
    {
        ENTITY_COMPONENT_ADDED,
        ENTITY_COMPONENT_REMOVE,
        ENTITY_INTEGRATE,
        ENTITY_SEGREGATE,
        ENTITY_ATTACHED,
        ENTITY_DETACHED,
    };
}

#endif /* Entity_hpp */
