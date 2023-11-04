//
//  clock.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 1/29/23.
//

#ifndef clock_hpp
#define clock_hpp

#include <stdio.h>
#include <chrono>
namespace kege{

    class Clock
    {
    private:

        using clock = std::chrono::high_resolution_clock;
        using interval = std::chrono::microseconds;
        using time_point = std::chrono::time_point< clock >;
        time_point _start;

    public:

        double elasped();
        double now();
        void update();
        void start();
        void stop();

        double _start_time;
        double _elasped;
    };

}

//#define kegeGetSystemTimeNow() ( std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() ) )
//#define kegeGetSystemTimeNow() std::chrono::duration_cast< std::chrono::milliseconds >( std::chrono::system_clock::now().time_since_epoch() ).count()
#endif /* clock_hpp */
