//
//  ActivePlayer.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/21/23.
//

#ifndef ActivePlayer_hpp
#define ActivePlayer_hpp

#include "../../ecs/EntitySystem.hpp"

namespace kege{ namespace component{

    struct ActivePlayer
    {
        bool _current;
    };

}}

namespace kege{ namespace system{

    class ActivePlayer : public kege::EntitySystem
    {
    public:

        void update( const double& dms );
        ActivePlayer( int priority );

        component::ActivePlayer* _previous;
        component::ActivePlayer* _current;
    };

}}
#endif /* ActivePlayer_hpp */
