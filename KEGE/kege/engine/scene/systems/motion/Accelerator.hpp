//
//  Accelerator.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/25/23.
//

#ifndef Accelerator_hpp
#define Accelerator_hpp

#include "../../ecs/EntitySystem.hpp"

namespace kege{ namespace system{

    class Accelerator : public kege::EntitySystem
    {
    public:

        void update( const double& dms );
        Accelerator( int priority );
    };

}}

namespace kege{ namespace component{

    struct RootObject{ bool dummy; };

}}
#endif /* Accelerator_hpp */
