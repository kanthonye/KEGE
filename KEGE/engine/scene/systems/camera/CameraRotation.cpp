//
//  CameraRotation.cpp
//  kege-rendering-engine
//
//  Created by Kenneth A Esdaile on 10/3/21.
//

#include "../../systems/include/systems.hpp"

namespace kege{namespace system{

    void CameraRotationX::update( const double& dms )
    {
        if ( _entities == nullptr ) return;

        component::Rotation*  rotation = nullptr;
        component::Transform* transform = nullptr;
        kege::quat R;
        enum{X,Y,Z};
        for ( Entity entity : *_entities )
        {
            rotation = entity.get< component::Rotation >();
            transform = entity.get< component::Transform >();

            /*
             * create a rotation around the worldUp vector.
             *
             * note: The worldUp vector is opposite the world's gravity vector. In
             * the Gravitation system this worldUp vector is updated every frame.
             */
            R = kege::quat( rotation->_angles[0], transform->worldUp() );

            /*
             * compute the forward vector and rotate it rought the worldUp vector.
             */
            transform->forward() = cross( transform->right(), transform->worldUp() );
            transform->forward() = normalize( rotate(R, transform->forward() ) );

            /*
             * compute the new right vector and local up vector.
             */
            transform->right() = kege::cross( transform->worldUp(), transform->forward());
            transform->up() = kege::cross( transform->forward(), transform->right());
        }
    }
    CameraRotationX::CameraRotationX( int priority )
    :   kege::EntitySystem( priority, "CameraRotationX" )
    {
        _components = createComponentBitmask< component::Transform, component::Camera >();
    }

}}


namespace kege{namespace system{

    void CameraRotationY::update( const double& dms )
    {
        if ( _entities == nullptr ) return;

        component::Rotation*  rotation = nullptr;
        component::Transform* transform = nullptr;
        kege::quat R;
        for ( Entity entity : *_entities )
        {
            rotation = entity.get< component::Rotation >();
            transform = entity.get< component::Transform >();

            /*
             * create a rotation around the local right vector.
             */
            R = kege::quat( rotation->_angles[1], transform->right() );

            //rotation->_angles[1] = kege::lerp(rotation->_angles[1], 0.0f, rotation->_sensitivity[1]);

//            /*
//             build a quaternion rotation around the updated transform->_right vector.
//             this rotation will be used to make the camera look up and down. and move
//             up-wards or down-wards.
//             */
//            kege::quat q = kege::quat( rotation->_angles[ 1 ], transform->rotation()[ kege::X_AXIS ] );
//
//            /*
//             using the UD (up-down) rotation. rotate the forward vector. this will be
//             again update the forward vector to look to its final direction
//             */
//            transform->rotation()[ kege::Z_AXIS ] = kege::normalize( rotate(q, transform->rotation()[ kege::Z_AXIS ]) );
//            //transform->rotation()[ kege::Y_AXIS ] = kege::normalize( rotate(q, transform->rotation()[ kege::Y_AXIS ]) );
//
//            /*
//             with the forward and right vector updated. we can use both to get the
//             the local up by using the cross product
//             */
//            transform->rotation()[ kege::Y_AXIS ] = cross( transform->rotation()[ kege::Z_AXIS ], transform->rotation()[ kege::X_AXIS ] );


//
            vec3 u = rotate( R, transform->up() );
            transform->forward() = cross( u, transform->right() );
//            transform->right() = cross( transform->forward(), u );
            transform->up() = cross( transform->right(), transform->forward() );

            //std::cout << normalize( transform->right() )<<'\n';
        }
    }
    CameraRotationY::CameraRotationY( int priority )
    :   kege::EntitySystem( priority, "CameraRotationY" )
    {
        _components = createComponentBitmask< component::Transform, component::Camera >();
    }

}}


namespace kege{namespace system{

    void CameraRotationZ::update( const double& dms )
    {
        if ( _entities == nullptr ) return;

        component::Rotation*  rotation = nullptr;
        component::Transform* transform = nullptr;

        for ( Entity entity : *_entities )
        {
            rotation = entity.get< component::Rotation >();
            transform = entity.get< component::Transform >();

            rotation->_angles[ kege::Z_AXIS ] = kege::lerp(rotation->_angles[ kege::Z_AXIS ], 0.0f, rotation->_sensitivity[ kege::Z_AXIS ]);

            /*
             build a quaternion rotation around the updated transform->_right vector.
             this rotation will be used to make the camera look up and down. and move
             up-wards or down-wards.
             */
            kege::quat R = kege::quat( rotation->_angles[ kege::Z_AXIS ], transform->rotation()[ kege::X_AXIS ] );

            /*
             using the R (up-down) rotation. rotate the forward vector. this will be
             again update the forward vector to look to its final direction
             */
            transform->rotation()[ kege::X_AXIS ] = rotate(R, transform->rotation()[ kege::X_AXIS ]);

            /*
             with the forward and right vector updated. we can use both to get the
             the local up by using the cross product
             */
            transform->rotation()[ kege::Y_AXIS ] = cross( transform->rotation()[ kege::X_AXIS ], transform->rotation()[ kege::Z_AXIS ] );
        }
    }
    CameraRotationZ::CameraRotationZ( int priority )
    :   kege::EntitySystem( priority, "CameraRotationZ" )
    {
        _components = createComponentBitmask< component::Transform, component::Camera >();
    }

}}




