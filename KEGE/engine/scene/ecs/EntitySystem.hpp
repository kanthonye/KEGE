//
//  EntitySystem.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/1/23.
//

#ifndef EntitySystem_hpp
#define EntitySystem_hpp
#define POOL_INDEX_8_BIT_INTEGER

#include <stdio.h>
#include <set>
#include <map>
#include <mutex>
#include <string>
#include "kege-string.hpp"
#include "cgm.hpp"
#include "../ecs/ECO.hpp"
#include "../ecs/ECM.hpp"
#include "../ecs/System.hpp"

namespace kege{

    class EntitySystem :public kege::System
    {
    public:


        virtual void operator()( const uint32_t& event, const EntityId& entity ){}
        virtual void operator()( const kege::Ref< Message >& message ){}

        void setEntitySet( std::set< EntityId >* entities );

        virtual bool has( const EntityId& entity )const;
        virtual void update( const double& dms ){}
        virtual void render( const double& dms ){}
        virtual void uninit(){}
        virtual bool init();

        uint64_t componentBitmask()const;
        uint32_t priority()const;
        virtual ~EntitySystem();

    protected:

        EntitySystem( uint32_t priority, const kege::string& name  );

    protected:

        std::set< EntityId >* _entities;
        kege::Bitmask _components;
        uint32_t _priority;
        bool _init;
    };
}
#endif /* EntitySystem_hpp */
