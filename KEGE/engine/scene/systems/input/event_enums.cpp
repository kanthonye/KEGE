//
//  Event.cpp
//  kege-rendering-engine
//
//  Created by Kenneth A Esdaile on 8/20/21.
//

#include "event_enums.hpp"
namespace kege
{
    static const char *strCommandTypes[] = { KEGE_EVENT_TYPES };
    const char* ToString( Event type )
    {
        return strCommandTypes[ type ];
    };
}
