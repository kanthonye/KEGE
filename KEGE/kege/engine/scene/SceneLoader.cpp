//
//  SceneLoader.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 7/23/23.
//

#include "KEGE.hpp"
#include "Core.hpp"
#include "ResMgr.hpp"
#include "FileReader.hpp"
#include "Graphics.hpp"
#include "SceneLoader.hpp"
#include "StdMesh.hpp"
#include "CubeSphereLandscape.hpp"
#include "../graphics/geometry/Cuboid.hpp"
#include "../graphics/geometry/Ellipsoid.hpp"

namespace kege
{
    static double planet_radius = 4001000;
    kege::Entity freeLookCameraEntity( kege::Ref< kege::Projection > projection )
    {
        kege::transform t = {};
        t._position.z = planet_radius + 500;

        kege::Entity entity = kege::Entity::create();
        entity.add< component::Input >({ new kege::PlayerInput() });
        entity.add< component::Camera >({ projection });
        entity.add< component::Rotation >({})->_angles = kege::vec3(180,0,0);
        entity.add< component::LinearMotion >();
        entity.add< component::MotionControl >();
        entity.add< component::Mass >({100.2, 1.0/100.2});
        entity.add< component::Transform >(t);
        return entity;
    }


    kege::Geometry* createGeometryInstance( const kege::MeshInfo& geom )
    {
        kege::Ref< core::Buffer > vbo = KEGE::core()->graphics()->createBuffer( core::Buffer::VERTEX, geom._vertex_data.size(), geom._vertex_data.data(), core::Buffer::STATIC );
        kege::Ref< core::Buffer > ibo = KEGE::core()->graphics()->createBuffer( core::Buffer::INDEX, geom._index_data.size(), geom._index_data.data(), core::Buffer::STATIC );

        std::vector< kege::Vertex::Binding > binding = {{ vbo, geom._vertex_stride, core::VertexInputRate::PER_VERTEX }};
        core::IndexBuffer* i = new core::IndexBuffer( geom._index_type, 0, geom._index_count, ibo );
        core::VertexArray* a = new core::VertexArrayIndexed( geom._attributes, binding, i );

        return new kege::Geometry( 0, geom._index_count, a );
    }


    
    bool loadScene( kege::Scene* scene )
    {
        kege::Entity camera = freeLookCameraEntity( new PerspectiveProjection( 1024.f/720.f, 65, 0.1, 1000000.0 ) );
        camera.add< component::ActivePlayer >({});
        camera.add< component::RootObject >({});
        camera.integrate();

        system::Camera::entity() = camera;

//        kege::Ref< kege::Material > material = new kege::Material({ new PBRShaderEffect({}) });
//        kege::Ref< kege::Geometry > cube = createGeometryInstance( kege::Cuboid( vec3::zero, kege::vec3( 30.f, 0.2f, 30.f ) ) );
//        kege::Ref< kege::StdMesh  > cube_mesh = new kege::StdMesh( cube, material );
//
//        kege::Entity cube_entity = scene->ecs().create();
//        cube_entity.add< component::Renderable >({ cube_mesh, 0, 0 });
//        cube_entity.add< component::Transform  >();
//        cube_entity.add< component::RootObject >();
//        cube_entity.integrate();
//        cube_entity.get< component::Transform  >()->_value._position[1] = -.2;

//        kege::Ref< kege::Geometry > geometry = ( CubicSphere( 10.f, kege::vec3( 0.f ) , kege::vec3( 1.f ) ) ).build();

        kege::Entity celestial_object = kege::Entity::create();
        celestial_object.add< component::Mass >({ 5000000.0, 1.0/5000000.0 });
        celestial_object.add< component::LinearMotion >({});
        celestial_object.add< component::Transform >({});
        celestial_object.add< component::RootObject >({});
        {
            kege::Entity terrain = kege::Entity::create();
            terrain.add< component::Landscape >({ new CubeSphereLandscape( planet_radius, 32.0, 16 ) });
            terrain.add< component::Transform >({});
            celestial_object.append( terrain );

            kege::Entity atmosphere = kege::Entity::create();
            atmosphere.add< component::Atmosphere >({ float(planet_radius), 200.0, 20, 8 });
            atmosphere.add< component::Transform >({});
            celestial_object.append( atmosphere );
        }
        celestial_object.integrate();
        return true;
    }
    
}
