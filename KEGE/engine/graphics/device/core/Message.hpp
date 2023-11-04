//
//  Message.hpp
//  CacheSystem
//
//  Created by Kenneth Esdaile on 3/12/23.
//

#ifndef Message_hpp
#define Message_hpp
#include <stdlib.h>
#include "Ref.hpp"
#include "glm.hpp"
#include "cgm.hpp"

namespace kege
{
    class Message : public kege::RefCounter
    {
    public:

        enum Type
        {
            USER_INPUT,
            WINDOW_EVENT,
            SYSTEM
        };

        Message( const Message::Type& type )
        :   _type( type )
        {}

        virtual ~ Message()
        {}

        uint32_t _type;
    };
}



namespace kege
{
    struct MsgSystem : public Message
    {
        enum Type
        {
            EXIT,
        };

        MsgSystem( Type type ): Message( Message::SYSTEM ), _systyp( type ) {};
        Type _systyp;
    };

    struct MsgSysExit : public MsgSystem
    {
        MsgSysExit(): MsgSystem( MsgSystem::EXIT ) {};
    };


    struct MsgUserInput : public Message
    {
        enum Type
        {
            KEYBOARD_KEY,
            MOUSE_KEY,
            MOUSE_POSITION,
            MOUSE_SCROLL
        };

        MsgUserInput( MsgUserInput::Type type )
        :   Message( Message::USER_INPUT )
        ,   _input( type )
        {};
        virtual ~MsgUserInput(){}

        MsgUserInput::Type _input;
    };

    struct MsgKeyboardKey : public MsgUserInput
    {
        MsgKeyboardKey( int32_t action, int32_t key, int32_t mods )
        :   kege::MsgUserInput( MsgUserInput::KEYBOARD_KEY )
        ,   _state( action )
        ,   _key( key )
        ,   _mods( mods )
        {}

        int32_t _state;
        int32_t _key;
        int32_t _mods;
    };

    struct MsgMouseKey : public MsgUserInput
    {
        MsgMouseKey( int32_t action, int32_t key, int32_t mods )
        :   MsgUserInput( MsgUserInput::MOUSE_KEY )
        ,   _state( action )
        ,   _key( key )
        ,   _mods( mods )
        {}

        int32_t _state;
        int32_t _key;
        int32_t _mods;
    };

    struct MsgMousePosition : public MsgUserInput
    {
        MsgMousePosition( double x, double y )
        :   MsgUserInput( MsgUserInput::MOUSE_POSITION )
        ,   _x( x )
        ,   _y( y )
        {}
        double _x, _y;
    };

    struct MsgScrollOffset : public MsgUserInput
    {
        MsgScrollOffset( double x, double y )
        :   MsgUserInput( MsgUserInput::MOUSE_SCROLL )
        ,   _x( x )
        ,   _y( y )
        {}
        double _x, _y;
    };
}



namespace kege
{
    struct MsgWindowEvent : public Message
    {
        enum Event
        {
            WINDOW_CLOSED,   WINDOW_RESIZE,
            WINDOW_POSITION, WINDOW_MAXIMIZE,
            WINDOW_FOCUSED,  WINDOW_REFRESH,
            FRAMEBUFFER_RESIZE, CURSOR_ENTERED,
        };

        MsgWindowEvent( MsgWindowEvent::Event event )
        :   kege::Message( Message::WINDOW_EVENT )
        ,   _event( event )
        {};
        virtual ~MsgWindowEvent(){}

        MsgWindowEvent::Event _event;
    };

    struct MsgWindowPosition : public MsgWindowEvent
    {
        MsgWindowPosition( int32_t x, int32_t y )
        :   MsgWindowEvent( MsgWindowEvent::WINDOW_POSITION )
        ,   _x( x )
        ,   _y( y )
        {};
        int32_t _x,_y;
    };

    struct MsgWindowResize : public MsgWindowEvent
    {
        MsgWindowResize( uint32_t width, uint32_t height )
        :   MsgWindowEvent( MsgWindowEvent::WINDOW_RESIZE )
        ,   _width( width )
        ,   _height( height )
        {};
        int32_t _width,_height;
    };

    struct MsgFramebufferSize : public MsgWindowEvent
    {
        MsgFramebufferSize( uint32_t width, uint32_t height )
        :   MsgWindowEvent( MsgWindowEvent::FRAMEBUFFER_RESIZE )
        ,   _width( width )
        ,   _height( height )
        {};
        int32_t _width,_height;
    };

    struct MsgCursorEnter : public MsgWindowEvent
    {
        MsgCursorEnter( bool x )
        :   MsgWindowEvent( MsgWindowEvent::CURSOR_ENTERED )
        ,   _state( x )
        {};

        bool _state;
    };

    struct MsgWindowRefresh : public MsgWindowEvent
    {
        MsgWindowRefresh()
        :   MsgWindowEvent( MsgWindowEvent::WINDOW_REFRESH )
        {};
    };

    struct MsgWindowFocus : public MsgWindowEvent
    {
        MsgWindowFocus( int x )
        :   MsgWindowEvent( MsgWindowEvent::WINDOW_FOCUSED )
        ,   _state( x )
        {};

        int _state;
    };

    struct MsgWindowMaximized : public MsgWindowEvent
    {
        MsgWindowMaximized( int x )
        :   MsgWindowEvent( MsgWindowEvent::WINDOW_MAXIMIZE )
        ,   _state( x )
        {};

        int _state;
    };

    struct MsgWindowClosed : public MsgWindowEvent
    {
        MsgWindowClosed()
        :   MsgWindowEvent( MsgWindowEvent::WINDOW_CLOSED )
        {};
    };


    typedef const kege::Ref< kege::Message >& MsgRef;
}
#endif /* Message_hpp */
