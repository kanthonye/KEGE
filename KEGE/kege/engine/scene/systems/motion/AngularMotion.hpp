//
//  AngularMotion.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/18/23.
//

#ifndef AngularMotion_hpp
#define AngularMotion_hpp

#include "../../ecs/EntitySystem.hpp"

namespace kege{ namespace system{

    class AngularMotion : public kege::EntitySystem
    {
    public:

        void update( const double& dms );
        AngularMotion( int priority );
    };

}}

namespace kege{ namespace component{

    struct AngularMotion
    {
        kege::fvec3 _netforce;
        kege::fvec3 _velocity;
        kege::mat33 _invmass;
        kege::mat33 _mass;
        kege::real  _damping;
    };

}}
#endif /* AngularMotion_hpp */
