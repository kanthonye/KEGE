//
//  Core.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 7/17/23.
//

#include "Channel.hpp"
#include "Core.hpp"
#include "Scene.hpp"
#include "Graphics.hpp"
#include "CallbackManager.hpp"
namespace kege{

    void Core::dispatchMessages()
    {
        std::queue< kege::Ref< kege::Message > > messages;
        {
            std::lock_guard< std::mutex > lock( _message_mutex );
            messages.swap( _messages );
        }

        while ( !messages.empty() )
        {
            kege::Ref<Message> message = messages.front();
            messages.pop();

            switch ( message->_type )
            {
                case kege::Message::SYSTEM:
                {
                    if ( ((MsgSysExit*)message.ref())->_systyp == kege::MsgSysExit::EXIT )
                    {
                        _running = false;
                    }
                    break;
                }

                case kege::Message::USER_INPUT:
                {
                    _scene->operator()( message );
                    break;
                }

                case kege::Message::WINDOW_EVENT:
                {
                    MsgWindowEvent* winmsg = (MsgWindowEvent*) message.ref();
                    switch ( winmsg->_event )
                    {
                        case kege::MsgWindowEvent::FRAMEBUFFER_RESIZE:
                        {
                            _scene->operator()( message );
                            break;
                        }

                        case kege::MsgWindowEvent::WINDOW_RESIZE:
                        {
                            _scene->operator()( message );
                            break;
                        }

                        case kege::MsgWindowEvent::WINDOW_MAXIMIZE:
                        {
                            // TODO: pause rendering
                            break;
                        }

                        case kege::MsgWindowEvent::WINDOW_CLOSED:
                        {
                                // TODO: stop running
                            break;
                        }

                        case kege::MsgWindowEvent::WINDOW_FOCUSED:
                        {
                            break;
                        }

                        case kege::MsgWindowEvent::WINDOW_REFRESH:
                        {
                            break;
                        }

                        case kege::MsgWindowEvent::WINDOW_POSITION:
                        {
                            // TODO: update window position
                            break;
                        }

                        case MsgWindowEvent::CURSOR_ENTERED:
                        {
                            break;
                        }
                    }
                    break;
                }

                default:
                {
                    KEGE_WARN( "can not handle messages of unknown type." );
                    break;
                }
            }
        }
    }
    
    void Core::receive( MsgRef message )
    {
        _messages.push( message );
    }
    bool Core::hasMessages()const
    {
        return !_messages.empty();
    }
    kege::Graphics* Core::graphics()
    {
        return _graphics;
    }
    kege::ResMgr* Core::resources()
    {
        return _resmgr;
    }
    kege::VirDir* Core::virdir()
    {
        return _virdir;
    }
    kege::Config* Core::config()
    {
        return _config;
    }
    kege::Scene* Core::scene()
    {
        return _scene;
    }
    kege::Clock* Core::clock()
    {
        return &_clock;
    }
    
    uint32_t Core::fps()
    {
        return _fps;
    }

    void Core::uninit()
    {
        if ( _scene )
        {
            _scene->uninit();
            delete _scene;
            _scene = nullptr;
        }

        if ( _graphics )
        {
            delete _graphics;
            _graphics = nullptr;
        }

        if ( _config )
        {
            delete _config;
            _config = nullptr;
        }

        if ( _virdir )
        {
            delete _virdir;
            _virdir = nullptr;
        }

        if ( _resmgr )
        {
            _resmgr->purge();
            delete _resmgr;
            _resmgr = nullptr;
        }

        kege::Channel::remove< MsgKeyboardKey,     kege::Ref< Message > >( this );
        kege::Channel::remove< MsgMouseKey,        kege::Ref< Message > >( this );
        kege::Channel::remove< MsgCursorEnter,     kege::Ref< Message > >( this );
        kege::Channel::remove< MsgMousePosition,   kege::Ref< Message > >( this );
        kege::Channel::remove< MsgScrollOffset,    kege::Ref< Message > >( this );
        kege::Channel::remove< MsgWindowResize,    kege::Ref< Message > >( this );
        kege::Channel::remove< MsgFramebufferSize, kege::Ref< Message > >( this );
        kege::Channel::remove< MsgWindowClosed,    kege::Ref< Message > >( this );
        kege::Channel::remove< MsgWindowMaximized, kege::Ref< Message > >( this );
        kege::Channel::remove< MsgWindowFocus,     kege::Ref< Message > >( this );
        kege::Channel::remove< MsgWindowRefresh,   kege::Ref< Message > >( this );
        kege::Channel::remove< MsgSysExit,         kege::Ref< Message > >( this );
    }

    bool Core::init()
    {
        // setup callbacks
        kege::Channel::add< MsgKeyboardKey,     kege::Ref< Message > >( &Core::receive, this );
        kege::Channel::add< MsgMouseKey,        kege::Ref< Message > >( &Core::receive, this );
        kege::Channel::add< MsgCursorEnter,     kege::Ref< Message > >( &Core::receive, this );
        kege::Channel::add< MsgMousePosition,   kege::Ref< Message > >( &Core::receive, this );
        kege::Channel::add< MsgScrollOffset,    kege::Ref< Message > >( &Core::receive, this );
        kege::Channel::add< MsgWindowResize,    kege::Ref< Message > >( &Core::receive, this );
        kege::Channel::add< MsgFramebufferSize, kege::Ref< Message > >( &Core::receive, this );
        kege::Channel::add< MsgWindowClosed,    kege::Ref< Message > >( &Core::receive, this );
        kege::Channel::add< MsgWindowMaximized, kege::Ref< Message > >( &Core::receive, this );
        kege::Channel::add< MsgWindowFocus,     kege::Ref< Message > >( &Core::receive, this );
        kege::Channel::add< MsgWindowRefresh,   kege::Ref< Message > >( &Core::receive, this );
        kege::Channel::add< MsgSysExit,         kege::Ref< Message > >( &Core::receive, this );

        // create and initializing config
        _config = new Config( "system-configurations" );
        _config->load( _app_path + "/assets/config/settings.config" );

        // create and initializing virtual directory
        _virdir = new kege::VirDir();
        {
            Config* config = &(Config&) (*_config)[ "directory" ];
            for(auto& a : config->elements() )
            {
                for(auto& b : *a.second )
                {
                    _virdir->dir( b->name(), b->getStr() );
                }
            }
        }

        // create and initializing graphics
        _graphics = new Graphics( this );
        {
            Config& config = (Config&) (*_config)[ "graphics" ];
            _graphics->setFullscreen( config[ "full-screen" ].getBool() );
            _graphics->setHeight( config[ "height" ].getU32() );
            _graphics->setWidth( config[ "width" ].getU32() );
            _graphics->setTitle( config[ "title" ].getStr() );
            _graphics->setFPS( config[ "FPS" ].getU32() );
        }
        if( !_graphics->init() )
        {
            KEGE_ERROR( "some went wrong while initalizing kege::Graphics." );
            return false;
        }

        // create and resource manager
        _resmgr = new ResMgr();

        // create and initializing scene
        _scene = new Scene( this, "" );
        {
            Config* config = &(Config&) (*_config)[ "scene" ];
            _scene->setFilename( (*config)[ "file" ].getStr() );
        }
        _scene->init();

        return true;
    }

    Core::~Core()
    {
        uninit();
    }
    Core::Core( const char* app_path )
    :   _graphics( nullptr )
    ,   _scene( nullptr )
    ,   _fps( 60 )
    ,   _app_path( app_path )
    {
    }

}
