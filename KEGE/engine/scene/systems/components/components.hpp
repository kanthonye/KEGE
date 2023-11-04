//
//  components.hpp
//  CacheSystem
//
//  Created by Kenneth Esdaile on 3/13/23.
//

#ifndef KEGE_ENTITY_COMPONENTS
#define KEGE_ENTITY_COMPONENTS
#include "ECO.hpp"
#include "cgm.hpp"
#include "Geometry.hpp"
#include "Geometry.hpp"
#include "ShaderEffect.hpp"
#include "../components/Camera.hpp"

namespace kege{

    template< typename C1, typename C2, typename C3, typename C4, typename C5, typename C6 > kege::Bitmask createComponentBitmask()
    {
        kege::Bitmask bitmask;
        bitmask.set( ECO::getComponetType< C1 >() );
        bitmask.set( ECO::getComponetType< C2 >() );
        bitmask.set( ECO::getComponetType< C3 >() );
        bitmask.set( ECO::getComponetType< C4 >() );
        bitmask.set( ECO::getComponetType< C5 >() );
        bitmask.set( ECO::getComponetType< C6 >() );
        return bitmask;
    }

    template< typename C1, typename C2, typename C3, typename C4, typename C5 > kege::Bitmask createComponentBitmask()
    {
        kege::Bitmask bitmask;
        bitmask.set( ECO::getComponetType< C1 >() );
        bitmask.set( ECO::getComponetType< C2 >() );
        bitmask.set( ECO::getComponetType< C3 >() );
        bitmask.set( ECO::getComponetType< C4 >() );
        bitmask.set( ECO::getComponetType< C5 >() );
        return bitmask;
    }

    template< typename C1, typename C2, typename C3, typename C4 > kege::Bitmask createComponentBitmask()
    {
        kege::Bitmask bitmask;
        bitmask.set( ECO::getComponetType< C1 >() );
        bitmask.set( ECO::getComponetType< C2 >() );
        bitmask.set( ECO::getComponetType< C3 >() );
        bitmask.set( ECO::getComponetType< C4 >() );
        return bitmask;
    }

    template< typename C1, typename C2, typename C3 > kege::Bitmask createComponentBitmask()
    {
        kege::Bitmask bitmask;
        bitmask.set( ECO::getComponetType< C1 >() );
        bitmask.set( ECO::getComponetType< C2 >() );
        bitmask.set( ECO::getComponetType< C3 >() );
        return bitmask;
    }

    template< typename C1, typename C2 > kege::Bitmask createComponentBitmask()
    {
        kege::Bitmask bitmask;
        bitmask.set( ECO::getComponetType< C1 >() );
        bitmask.set( ECO::getComponetType< C2 >() );
        return bitmask;
    }

    template< typename C1 > kege::Bitmask createComponentBitmask()
    {
        kege::Bitmask bitmask;
        bitmask.set( ECO::getComponetType< C1 >() );
        return bitmask;
    }

    //    enum type
    //    {
    //        ROOT_OBJECT,
    //        TRANSFORM,
    //        CHILDREN,
    //        CHILD,
    //        NAME,
    //        INPUT,
    //        CAMERA,
    //        RENDERABLE,
    //        MOVEMENT,
    //
    //        LINEAR_MOTION,
    //        ANGULAR_MOTION,
    //        SOLID_VOLUME,
    //        MOTION_CONTROL,
    //
    //        FLOATING_ORGIN,
    //
    //        ROTATION,
    //        ACTIVE_PLAYER,
    //        SPHERICAL_TERRAIN,
    //        SPHERICAL_ATMOSPHERE,
    //        SPHERICAL_OCEAN,
    //        CELESTIAL_BODY,
    //        MAX_COMPONENT_TYPES,
    //    };
    //
    //
    //    template< component::type T > struct Type
    //    {
    //        static component::type TYPE(){ return T; };
    //    };
}

namespace kege{ namespace component{

    typedef kege::transform Transform;
    typedef kege::Ref< kege::Projection > Camera;
    typedef kege::Ref< kege::Geometry > Geometry;
    typedef kege::Ref< kege::ShaderEffect > GeneralMaterial;
    typedef kege::Ref< kege::ShaderEffect > ObjectMaterial;
    typedef kege::Ref< kege::ShaderEffect > TerrainMaterial;

    struct Rotation
    {
        kege::vec3 _angles = 0.f;
        kege::vec3 _sensitivity = 0.25f;
    };


    struct SolidVolume
    {
        double _mass;
        double _restitution;
        double _friction;

        //kege::Collider* _collider;
    };


    struct SphericalTerrainMaterial
    {
        bool dummy;
    };

    struct SphericalAtmosphere
    {
        double _planet_radius;
        double _atmosphere_radius;
    };

    struct SphericalOcean
    {
    };

}}

#endif /* KEGE_ENTITY_COMPONENTS */
