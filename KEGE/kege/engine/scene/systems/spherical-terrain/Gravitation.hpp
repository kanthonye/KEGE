//
//  Gravitation.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/18/23.
//

#ifndef Gravitation_hpp
#define Gravitation_hpp

#include "../../ecs/EntitySystem.hpp"

namespace kege{ namespace system{
    
    class Gravitation : public kege::EntitySystem
    {
    public:

        void render( const double& dms );
        void update( const double& dms );
        Gravitation( int priority );
    };

}}
#endif /* Gravitation_hpp */
