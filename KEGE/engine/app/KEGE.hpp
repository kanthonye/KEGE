//
//  KEGE.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 7/17/23.
//

#ifndef KEGE_HPP
#define KEGE_HPP
#include <iostream>
int main(int argc, const char * argv[]);
namespace kege{
    class Core;
    class Graphics;
    class MainLoop;

    class KEGE
    {
    public:

        static kege::Graphics* graphics();
        static kege::Core* core();

    private:

        friend int ::main(int argc, const char * argv[]);
        void start( const char* app_path );
        ~KEGE();
        KEGE();

    private:

        kege::MainLoop* _mainloop;
        static kege::Core* _core;

    private:

        friend kege::MainLoop;
    };
}
#endif /* KEGE_HPP */
