//
//  LandscapeUpdater.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/15/23.
//

#ifndef LandscapeUpdater_hpp
#define LandscapeUpdater_hpp

#include "../../ecs/ECS.hpp"
#include "../../ecs/EntitySystem.hpp"
#include "../spherical-terrain/modules/SphericalQuadtreeTerrain.hpp"
#include "LandScape.hpp"

namespace kege{ namespace system{

    class LandscapeUpdater : public kege::EntitySystem
    {
    public:

        void render( const double& dms );
        void update( const double& dms );
        LandscapeUpdater( int priority );
    };
}}

#endif /* LandscapeUpdater_hpp */
