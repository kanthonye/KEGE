//
//  Input.cpp
//  kege-rendering-engine
//
//  Created by Kenneth A Esdaile on 3/9/22.
//

#include "../../systems/include/systems.hpp"

namespace kege{namespace system{

    void Input::operator()( const kege::Ref< Message >& message )
    {
        if ( message->_type == kege::Message::USER_INPUT )
        {
            const kege::MsgUserInput* m = ( (const kege::MsgUserInput*) message.ref() );

            switch ( m->_input )
            {
                case kege::MsgUserInput::KEYBOARD_KEY:{
                    const MsgKeyboardKey* k = (const MsgKeyboardKey*) m;
                    update( k->_key, k->_state != 0 );
                }break;

                case kege::MsgUserInput::MOUSE_KEY:{
                    const MsgMouseKey* k = (const MsgMouseKey*) m;
                    update( k->_key, k->_state != 0 );
                }break;

                case kege::MsgUserInput::MOUSE_SCROLL:{
                    const MsgScrollOffset* k = (const MsgScrollOffset*) m;
                    _mouse_scroll[0] = k->_x;
                    _mouse_scroll[1] = k->_y;
                }break;

                case kege::MsgUserInput::MOUSE_POSITION:{
                    const MsgMousePosition* k = (const MsgMousePosition*) m;
                    _mouse_position[0] = k->_x;
                    _mouse_position[1] = k->_y;
                }break;

                default:
                    break;
            }
        }
    }

    bool Input::bind( const int32_t& key, const int32_t& action, bool toggle )
    {
        if( _keybinds.find( key ) != _keybinds.end() )
        {
            return false;
        }

        Binds* binds;
        auto i = _actionbinds.find( action );
        if ( i == _actionbinds.end() )
        {
            binds = new Binds;
            binds->_toggle = toggle;
            binds->_action = action;
            binds->_states[0] = false;
            binds->_states[1] = false;
            _actionbinds[ action ] = binds;
        }
        else
        {
            binds = i->second;
        }
        _keybinds[ key ] = binds;
        return true;
    }

    void Input::update( const int32_t& key, const double& state )
    {
        auto i  = _keybinds.find( key );
        if ( i == _keybinds.end() ) return;

        Binds* binds = i->second;
        //if ( binds->_toggle && state ) binds->_states[0] = !binds->_states[0];
        //else
        binds->_states[0] = state;
        _activebinds.push_back( binds );

        //if ( binds->_states[0] != binds->_states[1] )
        //{
            //binds->_states[1] = binds->_states[0];
            //_activebinds.push_back( binds );
            //_ecs->systems().handle( binds->_action, {} );
        //}
    }

    void Input::update( const double& tick )
    {
        updateMousePosition();
        updateMouseScroll();

        if ( _entities )
        {
            for ( Entity entity : *_entities )
            {
                component::Input* input = entity.get< component::Input >();
                for ( const auto* a : _activebinds )
                {
                    input->ref()->set( a->_action, a->_states[0] );
                    //std::cout<< ToString((Event) a->_action)  <<" - " << input->get(a->_action) <<std::endl;
                }
            }
        }
        _activebinds.clear();
    }

    void Input::updateMousePosition()
    {
        kege::dvec2 d = _mouse_position - _mouse_prevpos;
        _mouse_prevpos = _mouse_position;

        if ( d[0] > 0 )
        {
            update( POINTER_X_POSITIVE, d[0] );
            _pointer_dir_states[ 0 ] = true;
        }
        else if ( _pointer_dir_states[ 0 ] )
        {
            update( POINTER_X_POSITIVE, 0.0 );
            _pointer_dir_states[ 0 ] = false;
        }

        else if ( d[0] < 0 )
        {
            update( POINTER_X_NEGATIVE, d[0] );
            _pointer_dir_states[ 1 ] = true;
        }
        else if ( _pointer_dir_states[ 1 ] )
        {
            update( POINTER_X_NEGATIVE, 0.0 );
            _pointer_dir_states[ 1 ] = false;
        }

        if ( d[1] > 0 )
        {
            update( POINTER_Y_POSITIVE, d[1] );
            _pointer_dir_states[ 2 ] = true;
        }
        else if ( _pointer_dir_states[ 2 ] )
        {
            update( POINTER_Y_POSITIVE, 0.0 );
            _pointer_dir_states[ 2 ] = false;
        }

        else if ( d[1] < 0 )
        {
            update( POINTER_Y_NEGATIVE, d[1] );
            _pointer_dir_states[ 3 ] = true;
        }
        else if ( _pointer_dir_states[ 3 ] )
        {
            update( POINTER_Y_NEGATIVE, 0.0 );
            _pointer_dir_states[ 3 ] = false;
        }

    }
    void Input::updateMouseScroll()
    {
        if ( _mouse_scroll[0] > 0 )
        {
            update( SCROLL_X_POSITIVE, _mouse_scroll[0] );
            _scroll_dir_states[ 0 ] = true;
        }
        else if ( _scroll_dir_states[ 0 ] )
        {
            update( SCROLL_X_POSITIVE, 0.0 );
            _scroll_dir_states[ 0 ] = false;
        }

        if ( _mouse_scroll[0] < 0 )
        {
            update( SCROLL_X_NEGATIVE, _mouse_scroll[0] );
            _scroll_dir_states[ 1 ] = true;
        }
        else if ( _scroll_dir_states[ 1 ] )
        {
            update( POINTER_X_NEGATIVE, 0.0 );
            _scroll_dir_states[ 1 ] = false;
        }

        if ( _mouse_scroll[1] > 0 )
        {
            update( SCROLL_Y_POSITIVE, _mouse_scroll[1] );
            _scroll_dir_states[ 2 ] = true;
        }
        else if ( _scroll_dir_states[ 2 ] )
        {
            update( POINTER_Y_POSITIVE, 0.0 );
            _scroll_dir_states[ 2 ] = true;
        }

        if ( _mouse_scroll[1] < 0 )
        {
            update( SCROLL_Y_NEGATIVE, _mouse_scroll[1] );
            _scroll_dir_states[ 3 ] = true;
        }
        else if ( _scroll_dir_states[ 3 ] )
        {
            update( SCROLL_Y_NEGATIVE, 0.0 );
            _scroll_dir_states[ 3 ] = true;
        }
    }

    
    Input::Input( int priority )
    :   kege::EntitySystem( priority, "Input" )
    ,   _mouse_position( 0.0 )
    ,   _mouse_prevpos( 0.0 )
    ,   _mouse_scroll( 0.0 )
    ,   _player( -1 )
    {
        _components = createComponentBitmask< component::Input >();
        
        _pointer_dir_states[0] = 0;
        _pointer_dir_states[1] = 0;
        _pointer_dir_states[2] = 0;
        _pointer_dir_states[3] = 0;
        _scroll_dir_states[0] = 0;
        _scroll_dir_states[1] = 0;
        _scroll_dir_states[2] = 0;
        _scroll_dir_states[3] = 0;
        
        //setRequirements< component::Input >();
        //_kmgr = new kege::KeybindManager();

        bind( SCROLL_Y_POSITIVE,  kege::EVENT_ZOOM_OUT );
        bind( SCROLL_Y_NEGATIVE,  kege::EVENT_ZOOM_IN);
        bind( KEY_UP,             kege::EVENT_MOVE_FORWARD );
        bind( KEY_DOWN,           kege::EVENT_MOVE_BACKWARD );
        bind( KEY_ENTER,          kege::EVENT_MOVE_UPWARD );
        bind( KEY_APOSTROPHE,     kege::EVENT_MOVE_DOWNWARD );
        bind( KEY_LEFT,           kege::EVENT_MOVE_LEFTWARD );
        bind( KEY_RIGHT,          kege::EVENT_MOVE_RIGHTWARD );
        bind( BUTTON_0,           kege::EVENT_CONTROL_CAMERA );
        bind( POINTER_X_POSITIVE, kege::EVENT_LOOK_RIGHT );
        bind( POINTER_X_NEGATIVE, kege::EVENT_LOOK_LEFT );
        bind( POINTER_Y_NEGATIVE, kege::EVENT_LOOK_DOWN );
        bind( POINTER_Y_POSITIVE, kege::EVENT_LOOK_UP );
        bind( KEY_L,              kege::EVENT_TOGGLE_WIREFRAME, true );
    }

    Input::~Input()
    {
        for (auto& a : _actionbinds)
        {
            delete a.second;
        }
        _actionbinds.clear();
    }
}}
