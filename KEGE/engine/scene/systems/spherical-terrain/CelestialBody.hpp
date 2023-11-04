//
//  CelestialBody.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/18/23.
//

#ifndef CelestialBody_hpp
#define CelestialBody_hpp

#include "../../ecs/EntitySystem.hpp"

namespace kege{ namespace system{

    class CelestialBody : public kege::EntitySystem
    {
    public:

        void render( const double& dms );
        void update( const double& dms );
        CelestialBody( int priority );
    };

}}

namespace kege{ namespace component{

    struct CelestialBody
    {
        double _mass;
    };
}}
#endif /* CelestialBody_hpp */
