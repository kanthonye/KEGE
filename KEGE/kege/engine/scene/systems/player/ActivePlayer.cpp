//
//  ActivePlayer.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/21/23.
//

#include "../../systems/include/systems.hpp"

namespace kege{ namespace system{

    void ActivePlayer::update( const double& dms )
    {
        if ( _entities == nullptr ) return;
        for ( Entity entity : *_entities )
        {
            component::ActivePlayer* component = entity.get< component::ActivePlayer >();
            if ( component->_current )
            {
                if ( component != _current )
                {
                    _previous->_current = false;
                    _previous = _current;
                    _current = component;
                }
            }
        }
    }
    ActivePlayer::ActivePlayer( int priority )
    :   kege::EntitySystem( priority, "ActivePlayer" )
    ,   _previous( nullptr )
    ,   _current( nullptr )
    {
        _components = createComponentBitmask< component::ActivePlayer >();
    }
}}
