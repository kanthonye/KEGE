//
//  CameraControl.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/21/23.
//

#include "../../systems/include/systems.hpp"

namespace kege{ namespace system{

    void CameraControl::update( const double& dms )
    {
        if ( !system::Camera::_entity ) return;

        component::Camera* camera = system::Camera::_entity.get< component::Camera >();
        if( camera )
        {
            component::Input* input = system::Camera::_entity.get< component::Input >();
            if( input )
            {
                component::Rotation* rotation = system::Camera::_entity.get< component::Rotation >();
                if( rotation )
                {
                    float control = input->ref()->get( kege::EVENT_CONTROL_CAMERA );
                    rotation->_angles[0]  = control * input->ref()->get( kege::EVENT_LOOK_LEFT  );
                    rotation->_angles[0] += control * input->ref()->get( kege::EVENT_LOOK_RIGHT );
                    rotation->_angles[1] += control * input->ref()->get( kege::EVENT_LOOK_UP    );
                    rotation->_angles[1] += control * input->ref()->get( kege::EVENT_LOOK_DOWN  );
                }
            }
        }
    }

    CameraControl::CameraControl( int priority )
    :   kege::EntitySystem( priority, "CameraControl" )
    {
    }

}}
