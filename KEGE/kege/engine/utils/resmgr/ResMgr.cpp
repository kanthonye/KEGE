//
//  ResMgr.cpp
//  kege-rendering-engine
//
//  Created by Kenneth A Esdaile on 8/9/22.
//

#include "ResMgr.hpp"
namespace kege
{
    void ResMgr::purge()
    {
        //_assets.purge();
        _loaders.purge();
        _resources.purge();
    }
    
    ResMgr::~ResMgr()
    {
        purge();
    }

    ResMgr::ResMgr()
    {}

}
