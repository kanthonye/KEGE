//
//  clock.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 1/29/23.
//
#include <chrono>
#include "clock.hpp"

namespace kege{

    double Clock::elasped()
    {
        return _elasped;
    }
    
    double Clock::now()
    {
        long microseconds = std::chrono::duration_cast< interval >( clock::now() - _start ).count();
        double microseconds_in_seconds = 1000000.0;//std::chrono::duration_cast< interval >( clock::now().time_since_epoch() ).count();
        return double( microseconds / microseconds_in_seconds );
    }

    void Clock::start()
    {
        _start = clock::now();
        nanosleep( (const struct timespec[]){{0, 50000L}}, NULL );
        _start_time = now();
        this->_elasped = 0.0;
    }

    void Clock::stop()
    {
        _start_time = 0.0;
    }

    void Clock::update()
    {
        if ( _start_time != 0.0 )
        {
            _elasped = now() - _start_time;
        }
    }
}

