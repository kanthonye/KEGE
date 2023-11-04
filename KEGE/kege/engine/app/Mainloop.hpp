//
//  Mainloop.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 7/17/23.
//

#ifndef KEGE_MAINLOOP_HPP
#define KEGE_MAINLOOP_HPP
#include "clock.hpp"
#include "Message.hpp"
#include "Scene.hpp"
#include "../app/Core.hpp"
#include "../graphics/device/core/Device.hpp"
namespace kege{

    class Core;

    class MainLoop
    {
    public:

        void wait( double seconds );
        void update( Core* core );
        void run( Core* core );
        ~MainLoop();
        MainLoop();

    private:

        double _remaining_seconds;
        double _frame_elapsed_time;
        double _frame_start_time;
        double _frame_end_time;
        double _frame_length;
        double _previous_time;
        double _current_time;
        double _delta;
    };

}
#endif /* KEGE_MAINLOOP_HPP */
