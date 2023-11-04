//
//  GraphicsWindow.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/11/23.
//

#include "cgm.hpp"
#include <GLFW/glfw3.h>
#include "Message.hpp"
#include "Channel.hpp"
#include "../../device/vulkan/Device.hpp"
#include "../../device/vulkan/Window.hpp"
namespace kege{namespace vk{

    void setKeyCallback
    (
        GLFWwindow *window,
        int key,
        int scancode,
        int action,
        int mods
    )
    {
        kege::Channel::broadcast< kege::MsgKeyboardKey, kege::Ref< Message > >
        (
            new kege::MsgKeyboardKey( action, key, mods )
        );
    }

    void setMouseButtonCallack
    (
        GLFWwindow * window,
        int button,
        int action,
        int mods
    )
    {
        kege::Channel::broadcast< kege::MsgMouseKey, kege::Ref< Message > >( new kege::MsgMouseKey( action, button, mods ) );
    }

    void setWindowRefreshCallback
    (
        GLFWwindow* window
    )
    {
        kege::Channel::broadcast< kege::MsgWindowRefresh, kege::Ref< Message > >( new kege::MsgWindowRefresh() );
    }

    void setCursorPosCallback
    (
        GLFWwindow* window,
        double xpos,
        double ypos
    )
    {
        kege::Channel::broadcast< kege::MsgMousePosition, kege::Ref< Message > >( new MsgMousePosition( xpos, ypos ) );
    }

    void setScrollCallback
    (
        GLFWwindow* window,
        double xpos,
        double ypos
    )
    {
        kege::Channel::broadcast< kege::MsgScrollOffset, kege::Ref< Message > >( new MsgScrollOffset( xpos, ypos ) );
    }

    void setWindowSizeCallback
    (
        GLFWwindow *window,
        int width,
        int height
    )
    {
        kege::Channel::broadcast< kege::MsgWindowResize, kege::Ref< Message > >( new MsgWindowResize( width, height ) );
    }

    void setWindowPosCallback
    (
        GLFWwindow *window,
        int xpos,
        int ypos
    )
    {
        kege::Channel::broadcast< kege::MsgWindowPosition, kege::Ref< Message > >( new MsgWindowPosition( xpos, ypos ) );
    }

    void setFramebufferSizeCallback
    (
        GLFWwindow *window,
        int width,
        int height
    )
    {
        kege::Channel::broadcast< kege::MsgFramebufferSize, kege::Ref< Message > >( new MsgFramebufferSize( width, height ) );

        //auto app = reinterpret_cast<vk::Device*>(glfwGetWindowUserPointer(window));
        //app->renderer()->onFramebufferResized();
    }

    void setWindowMaximizeCallback
    (
        GLFWwindow *window,
        int maximized
    )
    {
        kege::Channel::broadcast< kege::MsgWindowMaximized, kege::Ref< Message > >( new MsgWindowMaximized( maximized ) );
    }

    void setWindowFocusCallback
    (
        GLFWwindow *window,
        int focused
    )
    {
        kege::Channel::broadcast< kege::MsgWindowFocus, kege::Ref< Message > >( new MsgWindowFocus( focused ) );
    }

    void setWindowCloseCallback
    (
        GLFWwindow *window
    )
    {
        kege::Channel::broadcast< kege::MsgWindowClosed, kege::Ref< Message > >( new MsgWindowClosed() );
    }

    void setCursorEnterCallback
    (
        GLFWwindow *window,
        int state
    )
    {
        kege::Channel::broadcast< kege::MsgCursorEnter, kege::Ref< Message > >( new MsgCursorEnter( state ) );
    }
}}




namespace kege{namespace vk{

    GLFWwindow* Window::handle()
    {
        return _window;
    }
    
    bool Window::isOpen()const
    {
        return !glfwWindowShouldClose( _window );
    }

    Window::Window
    (
        uint32_t width,
        uint32_t height,
        const char* title,
        bool fullscreen
    )
    :   _window( nullptr )
    ,   _height( height )
    ,   _width( width )
    ,   _fullscreen( fullscreen )
    ,   _title( title )
    {
        //assert( _device == VK_NULL_HANDLE && "calling Device::construct() more than once" );
        _window = glfwCreateWindow( width, height, title, nullptr, nullptr);

        // initializeWindowInputCallback
        glfwSetKeyCallback( _window, setKeyCallback );
        glfwSetScrollCallback( _window, setScrollCallback );
        glfwSetCursorPosCallback( _window, setCursorPosCallback );
        glfwSetWindowSizeCallback( _window, setWindowSizeCallback );
        glfwSetWindowPosCallback( _window, setWindowPosCallback );
        glfwSetCursorEnterCallback( _window, setCursorEnterCallback );
        glfwSetMouseButtonCallback( _window, setMouseButtonCallack );
        glfwSetWindowCloseCallback( _window, setWindowCloseCallback );
        glfwSetWindowMaximizeCallback( _window, setWindowMaximizeCallback );
        glfwSetFramebufferSizeCallback( _window, setFramebufferSizeCallback );
        glfwSetWindowFocusCallback( _window, setWindowFocusCallback );
        glfwSetWindowRefreshCallback( _window, setWindowRefreshCallback );

        glfwSwapInterval(0);
        glfwMakeContextCurrent( _window );
    }

    Window::~Window()
    {
        if( _window )
        {
            glfwDestroyWindow( _window );
            _window = nullptr;
        }
    }

}}
