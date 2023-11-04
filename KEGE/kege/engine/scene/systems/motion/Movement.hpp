//
//  Movement.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/21/23.
//

#ifndef KEGE_MOVEMENT_SYSTEM_HPP
#define KEGE_MOVEMENT_SYSTEM_HPP
#include "cgm.hpp"
#include "../../ecs/EntitySystem.hpp"

namespace kege{ namespace component{

    struct Movement
    {
        Movement()
        :   acceleration( 0.0f )
        ,   velocity( 0.0f )
        ,   damping( 0.25f )
        ,   speed( 112.5f )
        {}
        kege::fvec3 acceleration;
        kege::fvec3 velocity;
        kege::real  damping;
        kege::fvec3 speed;
    };

}}

namespace kege{ namespace system{

    class Movement : public kege::EntitySystem
    {
    public:

        void update( const double& dms );
        Movement( int priority );
    };

}}
#endif /* KEGE_MOVEMENT_SYSTEM_HPP */
