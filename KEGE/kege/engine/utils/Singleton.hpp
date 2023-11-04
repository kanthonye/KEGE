//
//  Singleton.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/18/23.
//

#ifndef KEGE_CORE_SINGLETON
#define KEGE_CORE_SINGLETON

#include <stdio.h>
namespace kege{

    class KEGE;
    class App;

    /**
     Singleton restricts the instantiation of a class to one "single" instance.
     With this implementation it only allows one class to instantiation singleton
     instance of any object. This limits singleton instances begin created any where.
     The only class that is allowed to create singleton instances is the Engine. If
     any other object or function want to access a singleton instance it can only
     use the static get member function.
     @note If a singleton instance is not instantiation a null pointer will be
     returned. Thus if the engine did not create a singleton, it is not a singleton
     instance.
     @note Only the Engine is responsible for creating and destroying singleton instances.
     */
    class Singleton
    {
    public:

        template<typename T> static T* get()
        {
            return Singleton::Get< T >::_instance;
        }

    private:

        template<typename T> struct Get
        {
            static T* _instance;
        };

        template<typename T> static T* set(T* instance)
        {
            Singleton::Get< T >::_instance = instance;
            return instance;
        }

        template<typename T> static void destroy()
        {
            if( Singleton::Get< T >::_instance != nullptr )
            {
                delete Singleton::Get< T >::_instance;
                Singleton::Get< T >::_instance = nullptr;
            }
        }

    private:

        friend kege::KEGE;

    };

    template<typename T> T* Singleton::Get< T >::_instance = nullptr;
}
#endif /* KEGE_CORE_SINGLETON */
