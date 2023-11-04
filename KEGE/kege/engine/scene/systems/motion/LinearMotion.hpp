//
//  LinearMotion.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/18/23.
//

#ifndef KEGE_ENTITY_SYSTEM_LINEAR_MOTION_HPP
#define KEGE_ENTITY_SYSTEM_LINEAR_MOTION_HPP

#include "../../ecs/EntitySystem.hpp"

namespace kege{ namespace system{

    class LinearMotion : public kege::EntitySystem
    {
    public:

        void update( const double& dms );
        LinearMotion( int priority );
    };

}}

namespace kege{ namespace component{

    struct LinearMotion
    {
        kege::fvec3 _netforce = 0.f;
        kege::fvec3 _acceleration = 0.f;
        kege::fvec3 _velocity = 0.f;
        kege::real  _invmass  = 1.f;
        kege::real  _mass     = 1.f;
        kege::real  _damping  = 0.25f;
    };

    struct Mass
    {
        kege::real _mass = 1.f;
        kege::real _invmass = 1.f;
    };

    struct Motion
    {
        struct Linear
        {
            kege::fvec3 _forces = 0.f;
            kege::fvec3 _velocity = 0.f;
            kege::real  _damping  = 0.25f;
        };

        struct Angular
        {
            kege::fvec3 _forces;
            kege::fvec3 _velocity;
            kege::real  _damping;
        };

    };

}}
#endif /* KEGE_ENTITY_SYSTEM_LINEAR_MOTION_HPP */
