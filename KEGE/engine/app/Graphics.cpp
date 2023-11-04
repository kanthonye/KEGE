//
//  Graphics.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 7/17/23.
//

#include <GLFW/glfw3.h>
#include "../app/Core.hpp"
#include "../app/KEGE.hpp"
#include "../app/Graphics.hpp"
#include "../graphics/renderer/Renderer.hpp"
#include "../graphics/device/vulkan/Device.hpp"
namespace kege
{

    kege::Ref< core::Buffer > Graphics::createBuffer(core::Buffer::Type type, uint64_t size, const void *data, core::Buffer::Usage usage)
    {
        return device()->createBuffer( type, size, data, usage );
    }

    void Graphics::setTitle( const kege::string& title )
    {
        _title = title;
    }

    void Graphics::setFullscreen( uint32_t full_screen )
    {
        _full_screen = full_screen;
    }

    void Graphics::setHeight( uint32_t height )
    {
        _height = height;
    }

    void Graphics::setWidth( uint32_t width )
    {
        _width = width;
    }

    void Graphics::setFPS( uint32_t fps )
    {
        _fps = fps;
    }

    core::Window* Graphics::window()
    {
        return _window;
    }

    kege::ShaderLibrary& Graphics::shaderLibrary()
    {
        return _shaderlibrary;
    }

    core::Renderer* Graphics::renderer()
    {
        return _renderer;
    }

    core::Device* Graphics::device()
    {
        return _device;
    }

    uint32_t Graphics::height()const
    {
        return _height;
    }

    uint32_t Graphics::width()const
    {
        return _width;
    }

    void Graphics::pollEvents()const
    {
        glfwPollEvents();
    }

    bool Graphics::init()
    {
        assert( _device == nullptr && "can not call Graphics::init() more than once." );

        vk::Device* device = new vk::Device();
        vk::Window* window = new vk::Window( _width, _height, _title.c_str(), _full_screen );

        if ( !device->construct( window ) )
        {
            device->destruct();
            delete device;
            delete window;
            return false;
        }

        _renderer = device->renderer();
        _window = device->window();
        _device = device;

        if ( !_shaderlibrary.init() )
        {
            KEGE_ERROR( "failed to initialize ShaderLibrary" );
            return false;
        };
        
        return true;
    }

    Graphics::~Graphics()
    {
        _shaderlibrary.uninit();

        if ( _device )
        {
            delete _device;
            _device = nullptr;
        }
    }

    Graphics::Graphics( kege::Core* core )
    :   _window( nullptr )
    ,   _renderer( nullptr )
    ,   _device( nullptr )
    ,   _core( core )
    ,   _title("KEGE")
    ,   _full_screen( false )
    ,   _height( 512 )
    ,   _width( 1024 )
    ,   _fps( 60 )
    {
    }
}
