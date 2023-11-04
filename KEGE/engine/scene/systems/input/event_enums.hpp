//
//  Enums.hpp
//  kege-rendering-engine
//
//  Created by Kenneth A Esdaile on 8/20/21.
//

#ifndef KEGE_EventEnums_hpp
#define KEGE_EventEnums_hpp
#include "event_enums.txt"
#define enum_event_type(x) EVENT_##x
namespace kege
{
    enum Event
    { 
        KEGE_EVENT_TYPES
    };
}
#undef enum_event_type
#define enum_event_type(x) #x

namespace kege
{
    const char* ToString( Event type );
}
#endif /* KEGE_EventEnums_hpp */
