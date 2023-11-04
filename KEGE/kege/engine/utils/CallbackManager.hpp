//
//  CallbackManager.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 1/31/23.
//

#ifndef CallbackManager_hpp
#define CallbackManager_hpp
#include <unordered_map>
namespace kege{

    using Callbacks = std::unordered_map< int, std::function< void( const int&, const void* ) > >;
    
    class CallbackManager
    {
    public:

        template<class T> static void add( const int& event, void(T::*func)( const int&, const void* ), T* instance )
        {
            _callbacks.emplace( std::make_pair(event, std::bind( func, instance, std::placeholders::_1, std::placeholders::_2 )) );
        }
        static void add( const int& event, std::function< void( const int&, const void* ) >& func );
        static void call( const int& event, const void* package );
        static void remove( const int& event );
        static void purge();

    private:

        static kege::Callbacks _callbacks;
    };
}
#endif /* CallbackManager_hpp */
