//
//  SphericalQuadtreeTerrain.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/26/23.
//

#ifndef KEGE_SPHERICAL_TERRAIN_SYSTEM
#define KEGE_SPHERICAL_TERRAIN_SYSTEM

#include "../../ecs/ECS.hpp"
#include "../../ecs/EntitySystem.hpp"
#include "../spherical-terrain/modules/SphericalQuadtreeTerrain.hpp"

namespace kege{ namespace system{

    class SphericalQuadtreeTerrain : public kege::EntitySystem
    {
    public:

        void render( const double& dms );
        void update( const double& dms );
        SphericalQuadtreeTerrain( int priority );
    };
}}

namespace kege{ namespace component{

    typedef kege::Ref< kege::SphericalQuadtreeTerrain > SphericalQuadtreeTerrain;

}}
#endif /* KEGE_SPHERICAL_TERRAIN_SYSTEM */
