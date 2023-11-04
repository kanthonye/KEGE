//
//  CelestialObjectUpdater.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/15/23.
//

#ifndef CelestialObjectUpdater_hpp
#define CelestialObjectUpdater_hpp

#include "../../ecs/ECS.hpp"
#include "../../ecs/EntitySystem.hpp"
#include "../spherical-terrain/modules/SphericalQuadtreeTerrain.hpp"

namespace kege{ namespace system{

    class CelestialObjectUpdater : public kege::EntitySystem
    {
    public:

        void render( const double& dms );
        void update( const double& dms );
        CelestialObjectUpdater( int priority );
    };
}}

#endif /* CelestialObjectUpdater_hpp */
