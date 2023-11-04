//
//  GraphicsWindow.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/11/23.
//

#ifndef KEGE_VULKAN_WINDOW_HPP
#define KEGE_VULKAN_WINDOW_HPP
#include <stdlib.h>
#include "../../device/core/Window.hpp"
struct GLFWwindow;
namespace kege{namespace vk{

    class Device;
    
    class Window : public kege::core::Window
    {
    public:

        virtual const vk::Window* vulkan()const{return this;}
        virtual vk::Window* vulkan(){return this;}
        GLFWwindow* handle();

        bool isOpen()const;

        Window
        (
            uint32_t width,
            uint32_t height,
            const char* title,
            bool fullscreen
        );
        ~Window();

    private:

        GLFWwindow* _window;
        kege::string _title;
        bool _fullscreen;
        uint32_t _height;
        uint32_t _width;

        friend vk::Device;
    };
}}
#endif /* KEGE_VULKAN_WINDOW_HPP */
