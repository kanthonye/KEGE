//
//  FloatingOrigin.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/26/23.
//

#ifndef FloatingOrigin_hpp
#define FloatingOrigin_hpp

#include "../../ecs/EntitySystem.hpp"

namespace kege{ namespace system{

    class FloatingOrigin : public kege::EntitySystem
    {
    public:

        void update( const double& dms );
        FloatingOrigin( int priority );
        ~FloatingOrigin();
    };

}}

#endif /* FloatingOrigin_hpp */
