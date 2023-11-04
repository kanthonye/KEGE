//
//  CallbackManager.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 1/31/23.
//

#include "CallbackManager.hpp"
namespace kege{

    void CallbackManager::add( const int& event, std::function< void( const int&, const void* ) >& func )
    {
        _callbacks.emplace( std::make_pair(event, func) );
    }

    void CallbackManager::call( const int& event, const void* package )
    {
        Callbacks::iterator m = _callbacks.find( event );
        if( m != _callbacks.end() )
        {
            m->second( event, package );
        }
    }

    void CallbackManager::remove( const int& event )
    {
        if( _callbacks.find( event ) != _callbacks.end() )
        {
            _callbacks.erase( _callbacks.find( event ) );
        }
    }

    void CallbackManager::purge()
    {
        _callbacks.clear();
    }

    kege::Callbacks CallbackManager::_callbacks;
}
