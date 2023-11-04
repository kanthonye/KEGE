//
//  Scene.hpp
//  CacheSystem
//
//  Created by Kenneth Esdaile on 3/13/23.
//

#ifndef KEGE_SCENE_HPP
#define KEGE_SCENE_HPP
#include "Message.hpp"
#include "Core.hpp"

#include "systems/include/systems.hpp"
#include "../utils/resmgr/ResMgr.hpp"

namespace kege
{
    class ECS;
    class Scene;

    typedef kege::EntityId (*FactoryFunctionPtr)( kege::Scene* );
    typedef std::vector< std::pair<int32_t, FactoryFunctionPtr> > FactoryFunctCallbacks;
    typedef std::unordered_map< int32_t, FactoryFunctionPtr > Factory;

    class Scene
    {
    public:

        kege::EntityId spawnEntity( int32_t entity_type, const component::Transform& transform );
        void setFactory( const FactoryFunctCallbacks& callbacks );

        //std::vector< kege::ComponentId > getComponents( int32_t entity_type );
        //std::vector< kege::Entity > getEntities( int32_t entity_type );

        void setFilename( const kege::string& filename );

        virtual void operator()( const kege::Ref< Message >& message );
        virtual bool update( const double& dms );
        virtual bool render( const double& dms );
        virtual void uninit();
        virtual bool init();

        kege::Factory& factory();
        kege::ResMgr& resources();
        kege::Core& core();

        Scene( kege::Core* core, const kege::string& filename );
        virtual ~ Scene();

    protected:

        kege::string _filename;

        kege::Factory _factory;
        kege::ResMgr _resmgr;
        kege::Core* _core;
        bool _init;

        friend bool sceneLoader( kege::Scene* scene );
    };
}
#endif /* KEGE_SCENE_HPP */
