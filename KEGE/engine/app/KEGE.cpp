//
//  KEGE.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 7/17/23.
//

#include "../app/KEGE.hpp"
#include "../app/Core.hpp"
#include "../app/Mainloop.hpp"
namespace kege{

    kege::Graphics* KEGE::graphics()
    {
        return _core->graphics();
    }
    
    kege::Core* KEGE::core()
    {
        return _core;
    }
    
    void KEGE::start( const char* app_path )
    {
        _core = new Core( app_path );
        _mainloop = new MainLoop();

        try
        {
            _mainloop->run( _core );
        }
        catch ( const std::runtime_error& err )
        {
            std::cout <<"{ ERROR }: " << err.what() <<"\n";
        }

        delete _mainloop;
        delete _core;

        _mainloop = nullptr;
        _core = nullptr;
    }

    KEGE::~KEGE()
    {
    }

    KEGE::KEGE()
    :   _mainloop( nullptr )
    {}


    kege::Core* KEGE::_core = nullptr;
}
