//
//  Graphics.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 7/17/23.
//

#ifndef KEGE_GRAPHICS_HPP
#define KEGE_GRAPHICS_HPP
#include <map>
#include <vector>
#include "clock.hpp"
#include "Hashmap.hpp"
#include "Message.hpp"
#include "../graphics/device/core/Device.hpp"
#include "../graphics/device/core/Window.hpp"
#include "../app/ShaderLibrary.hpp"
namespace kege
{
    class Renderer;
    class Core;

    class Graphics
    {
    public:

        kege::Ref< core::Buffer > createBuffer(core::Buffer::Type type, uint64_t size, const void *data, core::Buffer::Usage usage);
        void setTitle( const kege::string& title );
        void setFullscreen( uint32_t full_screen );
        void setHeight( uint32_t height );
        void setWidth( uint32_t width );
        void setFPS( uint32_t fps );

        kege::ShaderLibrary& shaderLibrary();
        core::Renderer* renderer();
        core::Device* device();
        core::Window* window();
        uint32_t height()const;
        uint32_t width()const;
        void pollEvents()const;

        bool init();
        
    private:

        bool loadMasterShaderLibrary( const kege::string& name );
        Graphics( kege::Core* core );
        ~Graphics();

    private:

        kege::ShaderLibrary _shaderlibrary;

        core::Renderer* _renderer;
        core::Window* _window;
        core::Device* _device;
        kege::Core* _core;

        kege::string _title;
        bool _full_screen;
        uint32_t _height;
        uint32_t _width;
        uint32_t _fps;

    private:

        friend kege::Core;
    };

}
#endif /* KEGE_GRAPHICS_HPP */
