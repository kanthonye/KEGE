//
//  Camera.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/21/23.
//

#include "Core.hpp"
#include "KEGE.hpp"
#include "Graphics.hpp"
#include "ECS.hpp"
#include "../../systems/include/systems.hpp"
#include "../../systems/components/components.hpp"
#include "../../../graphics/renderer/Renderer.hpp"
namespace kege{namespace system{

    void Camera::render( const double& dms )
    {
        if ( !_entity ) return;

        kege::fmat44 rotation;
        kege::fmat44 translation;
        kege::CameraMatrices camera_matrices;
        
        component::Camera* camera = _entity.get< component::Camera >();

        if ( _entity.has< component::Child >() )
        {
            component::Child* child = _entity.get< component::Child >();
            rotation = kege::transpose( kege::fmat44( child->_transform._rotation ) );
            translation = kege::translate( kege::fvec3( -child->_transform._position ) );
            camera_matrices._position = child->_transform._position;
        }
        else
        {
            component::Transform* transform = _entity.get< component::Transform >();
            rotation = kege::transpose( kege::fmat44( transform->_rotation ) );
            translation = kege::translate( kege::fvec3( -transform->_position ) );
            camera_matrices._position = transform->_position;
        }

        camera_matrices._transform = translation * rotation;
        camera_matrices._projection = camera->ref()->get();
        camera_matrices._aspect_ratio = camera->ref()->aspectRatio();
        camera_matrices._fov = camera->ref()->fov();
        camera_matrices._near = camera->ref()->near();
        camera_matrices._far = camera->ref()->far();

        _resource->getBinding( 0 )->getBuffer( 0 )->copyToSystemMemory( 0, sizeof( CameraMatrices ), &camera_matrices );
    }
    
    void Camera::update( const double& dms )
    {
    }
    
    kege::Entity& Camera::entity()
    {
        return _entity;
    }

    Camera::Camera( int priority )
    :   kege::EntitySystem( priority, "Camera" )
    {
        _resource = new kege::UniformLayout1
        (
            new kege::BufferBinding
            (
                "CameraMatrices", 0,
                KEGE::core()->graphics()->createBuffer
                (
                    core::Buffer::UNIFORM,
                    sizeof( CameraMatrices ),
                    nullptr, core::Buffer::STREAM
                )
            )
        );
    }

    Camera::~Camera()
    {
        _resource.clear();
    }

    kege::Ref< core::UniformLayout > Camera::_resource;
    kege::Entity Camera::_entity = EntityId( -1 );
}}
