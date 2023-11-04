//
//  asserts.h
//  KEGE
//
//  Created by Kenneth Esdaile on 1/29/23.
//

#define KEGE_ASSERT_ENABLED

#ifndef asserts_h
#define asserts_h

namespace kege
{

    void report_assertion_failure(const char* expression, const char* message, const char* file, int line);
    
}


#ifdef KEGE_ASSERT_ENABLED
#define KEGE_DEBUG_BREAK() __builtin_trap()

    #define KEGE_ARRERT( expr )                                                     \
    {                                                                               \
        if( expr ){}                                                                \
        else                                                                        \
        {                                                                           \
            kege::report_assertion_failure( #expr, "", __FILE__, __LINE__ );    \
            KEGE_DEBUG_BREAK();                                                     \
        }                                                                           \
    }

    #define KEGE_ARRERT_MSG( expr, message )                                        \
    {                                                                               \
        if( expr ){}                                                                \
        else                                                                        \
        {                                                                           \
            kege::report_assertion_failure( #expr, message, __FILE__, __LINE__ );    \
            KEGE_DEBUG_BREAK();                                                     \
        }                                                                           \
    }

    #ifdef KEGE_DEBUG
        #define KEGE_ARRERT_DEBUG( expr )                                               \
        {                                                                               \
            if( expr ){}                                                                \
            else                                                                        \
            {                                                                           \
                kege::report_assertion_failure( #expr, message, __FILE__, __LINE__ );    \
                KEGE_DEBUG_BREAK();                                                     \
            }                                                                           \
        }
    #else
        #define KEGE_ARRERT_DEBUG( expr )
    #endif

#else
    #define KEGE_ARRERT( expr )
    #define KEGE_ARRERT_MSG( expr, message )
    #define KEGE_ARRERT_DEBUG( expr )
#endif

#endif /* asserts_h */
