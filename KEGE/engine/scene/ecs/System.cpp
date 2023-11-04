//
//  System.cpp
//  PoolSysm
//
//  Created by Kenneth Esdaile on 3/11/23.
//

#include "System.hpp"
namespace kege
{
    const kege::string& System::name()const
    {
        return _name;
    }

    bool System::init()
    {
        return true;
    }

    System::System( const kege::string& name  )
    :   _name(name)
    {
    }
    
    System:: ~System()
    {}

};
