//
//  CelestialBody.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/18/23.
//

#include "CelestialBody.hpp"

namespace kege{ namespace system{

    void CelestialBody::render( const double& dms )
    {}
    void CelestialBody::update( const double& dms )
    {}
    CelestialBody::CelestialBody( int priority )
    :   kege::EntitySystem( priority, "CelestialBody" )
    {
    }

}}
