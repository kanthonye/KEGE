//
//  Abstraction.h
//  KEGE
//
//  Created by Kenneth Esdaile on 6/11/23.
//
#include <assert.h>
#include "Message.hpp"
#include "PoolSYsm.hpp"
#ifndef KEGE_CORE_ABSTRACTION
#define KEGE_CORE_ABSTRACTION( classname ) \
        virtual ~classname(){}\
    protected:\
        classname& operator =( const kege::core::classname& ) = default;\
        classname( const kege::core::classname& ) = default;\
        classname() = default
#endif /* KEGE_CORE_ABSTRACTION */
