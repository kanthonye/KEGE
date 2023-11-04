//
//  Entity.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/24/23.
//

#include "kege-logger.hpp"
#include "../ecs/Entity.hpp"

namespace kege{

    kege::Entity Entity::getChild( const kege::string& name )
    {
        /*
         if this entity is not a parent it has no child.
         */
        component::Children* children = _ecs->getComponent< component::Children >( _entity_id );
        if ( !children )
        {
            return {};
        }

        /*
         if this entity is a parent find child. only entities with a comp::Tag can be retrieved.
         */
        component::Name*  cname;
        for (component::Child* child = head( children ) ; child != nullptr ; child = next( child ) )
        {
            cname = _ecs->getComponent< component::Name >( child->_entity );
            if ( !cname )
                continue;

            if ( *cname == name )
                return Entity( child->_entity );
        }

        /*
         if the entity is not in parent entity child, search child entities.
         */
        kege::Entity entity;
        for (component::Child* child = head( children ) ; child != nullptr ; child = next( child ) )
        {
            entity = this->getChild( name );
            if ( entity )
            {
                return entity;
            }
        }

        /* entity not found.
         */
        return {};
    }

    kege::Entity& Entity::prepend( kege::Entity& entity )
    {
        if ( !entity )
        {
            KEGE_ERROR( "attemping to prepending ( attach ) non-entity as child" );
            return entity;
        };

        /* if this entity is not a parent make it a parent
         */
        component::Children* children = _ecs->getComponent< component::Children >( _entity_id );
        if ( !children )
        {
            children = _ecs->addComponent< component::Children >( _entity_id );
            children->_head = KEGE_INVALID_POOL_INDEX;
            children->_tail = KEGE_INVALID_POOL_INDEX;
            children->_count = 0;
        }

        /*
         attach child-component to the child entity
         */
        component::Child* child = _ecs->addComponent< component::Child >( entity._entity_id );
        child->_next = KEGE_INVALID_POOL_INDEX;
        child->_prev = KEGE_INVALID_POOL_INDEX;
        child->_entity = entity._entity_id;
        child->_parent = this->_entity_id;

        /*
         get the component id of the  child-component
         */
        if ( children->_head )
        {
            /* set head and tail component id to child component id
             */
            children->_head = children->_tail = entity._entity_id;
        }
        else
        {
            component::Child* head = _ecs->getComponent< component::Child >( children->_head );

            // connect the new child before the head of the list
            head->_prev = entity._entity_id;
            child->_next = children->_head;

            // update the new child as the head of the list
            children->_head = entity._entity_id;
        }

        // increase the child count
        children->_count++;

        // post message of new entity child added
        _ecs->post( kege::ENTITY_ATTACHED, entity._entity_id );
        return entity;
    }

    kege::Entity& Entity::append( kege::Entity& entity )
    {
        if ( !entity )
        {
            KEGE_ERROR( "attemping to appending ( attach ) non-entity as child" );
            return entity;
        };

        /* if this entity is not a parent make it a parent
         */
        component::Children* children = _ecs->getComponent< component::Children >( _entity_id );
        if ( !children )
        {
            children = _ecs->addComponent< component::Children >( _entity_id );
            children->_head = KEGE_INVALID_POOL_INDEX;
            children->_tail = KEGE_INVALID_POOL_INDEX;
            children->_count = 0;
        }

        /*
         attach child-component to the child entity
         */
        component::Child* child = _ecs->addComponent< component::Child >( entity._entity_id );
        child->_next = KEGE_INVALID_POOL_INDEX;
        child->_prev = KEGE_INVALID_POOL_INDEX;
        child->_entity = entity._entity_id;
        child->_parent = this->_entity_id;

        /* get the component id of the  child-component
         */
        if ( !children->_head )
        {
            /* set head and tail component id to child component id
             */
            children->_tail = children->_head = entity._entity_id;
        }
        else
        {
            component::Child* tail = _ecs->getComponent< component::Child >( children->_tail );;

            // add child to tail of the list
            tail->_next = entity._entity_id;
            child->_prev = tail->_entity;

            // update the child as tail of the list
            children->_tail = entity._entity_id;
        }

        // increase child count
        children->_count++;

        // post message of new entity child added
        _ecs->post( kege::ENTITY_ATTACHED, entity._entity_id );
        return entity;
    }

    kege::Entity Entity::getParent()const
    {
        const component::Child* child = _ecs->getComponent< component::Child >( _entity_id );
        if ( !child ) return {};
        return Entity( child->_parent );
    }

    kege::Entity Entity::getRoot()const
    {
        const component::Child* child = _ecs->getComponent< component::Child >( _entity_id );
        EntityId eid = child->_parent;
        while ( child )
        {
            child = _ecs->getComponent< component::Child >( child->_parent );
            if ( !child )
            {
                eid = child->_parent;
            }
        }
        return Entity( eid );
    }

    bool Entity::isParent()const
    {
        return _ecs->hasComponent< component::Children >( _entity_id );
    }

    bool Entity::isChild()const
    {
        return _ecs->hasComponent< component::Child >( _entity_id );
    }

    void Entity::detachHead()
    {
        component::Children* children = _ecs->getComponent< component::Children >( _entity_id );
        if ( !children )
        {
            component::Child* head = _ecs->getComponent< component::Child >( children->_head );
            children->_head = head->_next;

            if ( _ecs->hasComponent< component::Child >( head->_next ) )
            {
                component::Child* next = _ecs->getComponent< component::Child >( head->_next );
                next->_prev = KEGE_INVALID_POOL_INDEX;
            }
        }
    }

    void Entity::detachTail()
    {
        component::Children* children = _ecs->getComponent< component::Children >( _entity_id );
        if ( !children )
        {
            component::Child* tail = _ecs->getComponent< component::Child >( children->_tail );
            children->_tail = tail->_prev;
            if ( _ecs->hasComponent< component::Child >( tail->_prev ) )
            {
                component::Child* prev = _ecs->getComponent< component::Child >( tail->_prev );
                prev->_next = KEGE_INVALID_POOL_INDEX;
            }
        }
    }

    void Entity::detachAll()
    {
        if ( _ecs->hasComponent< component::Children >( _entity_id ) )
        {
            component::Children* children = _ecs->getComponent< component::Children >( _entity_id );
            for (component::Child* child = head( children ) ; child != nullptr ; child = next( child ) )
            {
                _ecs->eraseComponent< component::Child >( child->_entity );
            }
            _ecs->eraseComponent< component::Children >( _entity_id );
        }
    }

    void Entity::detach()
    {
        if ( _ecs->hasComponent< component::Child >( _entity_id ) )
        {
            component::Child* child = _ecs->getComponent< component::Child >( _entity_id );
            component::Children* children = _ecs->getComponent< component::Children >( child->_entity );
            if ( child->_entity == children->_head )
            {
                children->_head = child->_next;
                component::Child* prev = _ecs->getComponent< component::Child >( child->_prev );
                if ( prev )
                {
                    prev->_prev = KEGE_INVALID_POOL_INDEX;
                }
            }
            else if( child->_entity == children->_tail )
            {
                children->_tail = child->_prev;
                component::Child* next = _ecs->getComponent< component::Child >( child->_next );
                if ( next )
                {
                    next->_prev = KEGE_INVALID_POOL_INDEX;
                }
            }
            else
            {
                component::Child* prev = _ecs->getComponent< component::Child >( child->_prev );
                component::Child* next = _ecs->getComponent< component::Child >( child->_next );
                prev->_next = next->_entity;
                next->_prev = prev->_entity;
            }
        }
    }

    void Entity::integrate( EntityId eid )
    {
        component::Children* children = _ecs->getComponent< component::Children >( eid );
        if ( children )
        {
            for (component::Child* child = head( children ) ; child != nullptr ; child = next( child ) )
            {
                integrate( child->_entity );
            }
        }
        _ecs->integrate( eid );
    }

    void Entity::integrate()
    {
        integrate( _entity_id );
    }

    void Entity::segregate( EntityId eid )
    {
        component::Children* children = _ecs->getComponent< component::Children >( eid );
        if ( children )
        {
            for (component::Child* child = head( children ) ; child != nullptr ; child = next( child ) )
            {
                segregate( child->_entity );
            }
        }
        _ecs->segregate( eid );
    }

    void Entity::segregate()
    {
        segregate( _entity_id );
    }

    void Entity::destroy()
    {
        destroy( _entity_id );
    }

    void Entity::purge()
    {
        _ecs->clear( _entity_id );
    }

    Entity::operator bool()const
    {
        return _entity_id;
    }

    component::Child* Entity::head( component::Children* component )
    {
        return _ecs->getComponent< component::Child >( component->_head );
    }

    component::Child* Entity::tail( component::Children* component )
    {
        return _ecs->getComponent< component::Child >( component->_tail );
    }

    component::Child* Entity::next( component::Child* component )
    {
        return _ecs->getComponent< component::Child >( component->_next );
    }

    component::Child* Entity::prev( component::Child* component )
    {
        return _ecs->getComponent< component::Child >( component->_prev );
    }

    bool Entity::init( const EntityInitInfo& info )
    {
        if ( !_ecs )
        {
            _ecs = new ECS( info._entity_pool_growsize, info._max_distinct_pool_types, info._pool_infos );
            return true;
        }
        return _ecs != nullptr;
    }

    kege::Entity Entity::create()
    {
        return Entity( _ecs->create() );
    }

    kege::ESM* Entity::systems()
    {
        return &_ecs->esm();
    }

    kege::EGM* Entity::egm()
    {
        return &_ecs->egm();
    }

    kege::ECM* Entity::ecm()
    {
        return &_ecs->ecm();
    }

    void Entity::destroy( EntityId eid )
    {
        component::Children* children = _ecs->getComponent< component::Children >( eid );
        if ( children )
        {
            for (component::Child* child = head( children ) ; child != nullptr ; child = next( child ) )
            {
                destroy( child->_entity );
            }
        }
        _ecs->destroy( eid );
    }

    void Entity::uninit()
    {
        if ( _ecs )
        {
            delete _ecs;
            _ecs = nullptr;
        }
    }

    Entity& Entity::operator=( const EntityId& id )
    {
        _entity_id = id;
        return *this;
    }

    EntityId Entity::id()const
    {
        return _entity_id;
    }
    
    Entity::Entity( const Entity& entity )
    :   _entity_id( entity._entity_id )
    {}

    Entity::Entity( const EntityId& id )
    :   _entity_id( id )
    {}

    Entity::Entity()
    :   _entity_id()
    {}


    kege::ECS* Entity::_ecs = nullptr;
}
