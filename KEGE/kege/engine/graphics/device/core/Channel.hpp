//
//  Channel.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 7/21/23.
//

#ifndef Channel_hpp
#define Channel_hpp
#include <vector>
#include <algorithm>
#include <functional>
#include <mutex>
#include <iostream>
namespace kege
{
    class Channel
    {
    private:

        /**
         *  The C signifies the Channel and the M signifies the message. Messages are sent through a channel which multiple listeners maybe listening to.
         *  The reason I why both C and M are necessary is because my message system has an inheritance hierarchy and messages will be sent as the
         *  base-message class. I need to use the base-message class-name to pass messages of all kinds, and use C as a key to select the message
         *  channel to broadcast the message on
         */
        template < typename C, typename M > class InternalChannel
        {
        public:

            static InternalChannel& instance()
            {
                static InternalChannel result;
                return result; // return a reference to an internal static
            }

            template<class T> void add( void(T::*func)( const M& ), T* instance )
            {
                _callbacks.push_back([func, instance](const M& message) {
                    (instance->*func)(message);
                });
                _listeners.push_back( instance );
            }

            void broadcast( const M& m )
            {
                std::vector< Callback > local_queue( _callbacks.size() );
                {
                    std::lock_guard< std::mutex > lock( _mutex );
                    local_queue = _callbacks;
                }

                for ( auto& callback : local_queue )
                    callback( m );
            }

            void remove( const void* ptr )
            {
                std::lock_guard< std::mutex > lock( _mutex );

                auto it = std::find( _listeners.begin(), _listeners.end(), ptr );
                if ( it == _listeners.end() )
                {
                    return;
                }

                auto idx = it - _listeners.begin();

                _callbacks.erase( _callbacks.begin() + idx );
                _listeners.erase( it );
            }

            void purge()
            {
                _callbacks.clear();
                _listeners.clear();
            }

        private:

            using Callback = std::function< void( const M& ) >;
            std::vector< Callback > _callbacks;
            std::vector< const void* > _listeners;
            std::mutex _mutex;
        };

    public:

        template < typename C, typename M, typename T > static void add( void(T::*func)( const M& ), T* instance )
        {
            InternalChannel< C, M >::instance().template add< T >( func, instance );
        }

        template < typename C, typename M, typename T > static void remove( T* t )
        {
            InternalChannel< C, M >::instance().remove( t );
        }

        template < typename C, typename M > static void broadcast( const M& m )
        {
            InternalChannel< C, M >::instance().broadcast( m );
        }
    };
}
#endif /* Channel_hpp */
