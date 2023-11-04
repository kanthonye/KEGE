//
//  Mainloop.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 7/17/23.
//

#include "../app/Core.hpp"
#include "kege-logger.hpp"
#include "Graphics.hpp"
#include "Mainloop.hpp"
namespace kege{

    void MainLoop::update( Core* core )
    {
        core->clock()->update();

        // begin frame time interval
        _frame_start_time = core->clock()->now();
        _frame_length = core->fps() / 1000.0;
        
        _delta = _current_time - _previous_time;

        if ( !core->scene()->update( 16.016 ) )
        {
            KEGE_FATAL( "Engine unable to update." );
        }
        if ( !core->scene()->render( _delta ) )
        {
            KEGE_FATAL( "Engine unable to render." );
        }

        _previous_time = _current_time;
        _current_time = core->clock()->elasped();

        // end frame time interval
        _frame_end_time = core->clock()->now();
        _frame_elapsed_time = _frame_end_time - _frame_start_time;
        _remaining_seconds = _frame_length - _frame_elapsed_time;

        //wait( remaining_seconds );
    }

    void MainLoop::run( Core* core )
    {
        if ( !core->init() )
        {
            return;
        }

        core->_running = true;
        while ( core->_running && core->graphics()->window()->isOpen() )
        {
            if ( core->hasMessages() )
            {
                core->dispatchMessages();
            }

            else //if( !core->suspended() )
            {
                update( core );
                //core->graphics()->device()->waitIdle();
            }
            core->graphics()->pollEvents();
        }

        core->uninit();
    }

    MainLoop::~MainLoop()
    {
    }

    MainLoop::MainLoop()
    {
    }

}
