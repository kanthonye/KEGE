//
//  Scene.cpp
//  CacheSystem
//
//  Created by Kenneth Esdaile on 3/13/23.
//

#include "ECS.hpp"
#include "Scene.hpp"
#include "SceneLoader.hpp"
namespace kege
{
    kege::EntityId Scene::spawnEntity( int32_t entity_type, const component::Transform& transform )
    {
        if ( _factory.find( entity_type ) == _factory.end() )
        {
            _factory[ entity_type ]( this );
        }
        return {};
    }

    void Scene::setFactory( const FactoryFunctCallbacks& callbacks )
    {
        for (const auto& a : callbacks )
        {
            _factory[ a.first ] = a.second;
        }
    }

    void Scene::setFilename( const kege::string& filename )
    {
        _filename = filename;
    }
    
    void Scene::operator()( const kege::Ref< Message >& message )
    {
        Entity::systems()->operator()( message );
    }

    bool Scene::update( const double& dms )
    {
        Entity::systems()->update( dms );
        return true;
    }

    bool Scene::render( const double& dms )
    {
        Entity::systems()->render( dms );
        return true;
    }

    void Scene::uninit()
    {
        Entity::systems()->uninit();
        Entity::uninit();
    }

    bool Scene::init()
    {
        std::vector< kege::PoolInfo > poolsizes =
        {
            {kege::ECO::getComponetType< component::RootObject >(),          500},
            {kege::ECO::getComponetType< component::Transform >(),           500},
            {kege::ECO::getComponetType< component::Children >(),            500},
            {kege::ECO::getComponetType< component::Child >(),               500},
            {kege::ECO::getComponetType< component::Camera >(),               10},
            {kege::ECO::getComponetType< component::Renderable >(),          500},
            {kege::ECO::getComponetType< component::Input >(),               500},
            {kege::ECO::getComponetType< component::Rotation >(),             10},

            {kege::ECO::getComponetType< component::LinearMotion >(),        200},
            {kege::ECO::getComponetType< component::AngularMotion >(),       200},
            {kege::ECO::getComponetType< component::MotionControl >(),       200},
            {kege::ECO::getComponetType< component::SolidVolume >(),         200},

            {kege::ECO::getComponetType< component::Landscape >(),    100},
            {kege::ECO::getComponetType< component::SphericalQuadtreeTerrain >(),    100},
            {kege::ECO::getComponetType< component::SphericalOcean >(),      100},
            {kege::ECO::getComponetType< component::SphericalAtmosphere >(), 100},

            {kege::ECO::getComponetType< component::ActivePlayer >(),         10},
            {kege::ECO::getComponetType< component::Mass >(),                500},
            {kege::ECO::getComponetType< component::Atmosphere >(),          500},
        };

        if ( !Entity::init({ 500, KEGE_MAX_COMPONENTS_PER_ENTITY, poolsizes }) )
        {
            return false;
        }

        Entity::systems()->add( new system::Input( 0 ) );
        Entity::systems()->add( new system::ActivePlayer( 0 ) );
        Entity::systems()->add( new system::FloatingOrigin( 0 ) );
        Entity::systems()->add( new system::Gravitation( 0 ) );
        Entity::systems()->add( new system::MovementControl( 0 ) );
        Entity::systems()->add( new system::CameraControl( 0 ) );
        Entity::systems()->add( new system::Accelerator( 0 ) );
        Entity::systems()->add( new system::CameraRotationX( 0 ) );
        Entity::systems()->add( new system::CameraRotationY( 0 ) );
        Entity::systems()->add( new system::LinearMotion( 0 ) );
        Entity::systems()->add( new system::LandscapeUpdater( 0 ) );
        Entity::systems()->add( new system::SphericalQuadtreeTerrain( 0 ) );
        Entity::systems()->add( new system::Hierarchy( 0 ) );
        //Entity::systems()->add( new system::BeginFrame( 0 ) );
        Entity::systems()->add( new system::Camera( 0 ) );
        Entity::systems()->add( new system::Render( 0 ) );
        //Entity::systems()->add( new system::ObjectRenderingSystem( 0 ) );
        //Entity::systems()->add( new system::SphericalTerrainRenderingSystem( 0 ) );
        //Entity::systems()->add( new system::SphericalAtmosphereRenderingSystem( 0 ) );
        //Entity::systems()->add( new system::SphericalOceanRenderingSystem( 0 ) );
        //Entity::systems()->add( new system::BillboardParticleRenderingSystem( 0 ) );
        //Entity::systems()->add( new system::EndFrame( 0 ) );
        if ( !Entity::systems()->init() )
        {
            return false;
        };

        _init = loadScene( this );
        // TODO: load scene
        return _init;
    }

    kege::Factory& Scene::factory()
    {
        return _factory;
    }
    kege::ResMgr& Scene::resources()
    {
        return _resmgr;
    }
    kege::Core& Scene::core()
    {
        return *_core;
    }

    Scene::Scene( kege::Core* core, const kege::string& filename )
    :   _core( core )
    ,   _filename( filename )
    ,   _init( false )
    {
    }

    Scene::~Scene()
    {
    }

}

