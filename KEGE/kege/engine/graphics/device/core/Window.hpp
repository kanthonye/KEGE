//
//  Window.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/11/23.
//

#ifndef KEGE_CORE_WINDOW_HPP
#define KEGE_CORE_WINDOW_HPP
#include <stdlib.h>
namespace kege{namespace vk{
    class Window;
}}

namespace kege{namespace core{

    struct CreateRenderWindowInfo
    {
        uint32_t _width;
        uint32_t _height;
        const char* _title;
        bool _fullscreen;
    };

    class Window
    {
    public:

        virtual const vk::Window* vulkan()const{return nullptr;}
        virtual vk::Window* vulkan(){return nullptr;}

        virtual bool isOpen()const = 0;
        virtual ~Window(){}
    };
}}
#endif /* KEGE_CORE_WINDOW_HPP */
