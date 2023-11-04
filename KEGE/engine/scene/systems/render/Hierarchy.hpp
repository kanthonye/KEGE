//
//  Hierarchy.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/3/23.
//

#ifndef Hierarchy_hpp
#define Hierarchy_hpp

#include "../../ecs/EntitySystem.hpp"

namespace kege{ namespace system{

    class Hierarchy : public kege::EntitySystem
    {
    public:

        void updateChildren( component::Child* parent );
        void update( const double& dms );
        Hierarchy( int priority );
    };

}}
#endif /* Hierarchy_hpp */
