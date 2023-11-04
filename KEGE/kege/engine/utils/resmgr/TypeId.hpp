//
//  Type.h
//  kege-rendering-engine
//
//  Created by Kenneth A Esdaile on 8/9/22.
//

#ifndef KEGE_TYPE_ID_GENERATOR
#define KEGE_TYPE_ID_GENERATOR
#include <mutex>
namespace kege
{
    template<typename T> class Type 
    {
    protected:
        static int genTypeId(){
            return _counter++;
        }
        static std::mutex _mutex;
        static uint32_t _counter;
    };

    template<typename T, typename G> struct TypeId : private Type< T >
    {
    public:

        static int getEnumTypeId(){ return _type; }

    private:

        static int _type;
    };

    template<typename T> uint32_t Type< T >::_counter = 0;
    template<typename T> std::mutex Type< T >::_mutex;
    template<typename T, typename G> int TypeId< T, G >::_type = Type< T >::genTypeId();
}
#endif /* KEGE_TYPE_ID_GENERATOR */
