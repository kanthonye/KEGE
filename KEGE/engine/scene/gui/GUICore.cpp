//
//  GUICore.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/11/23.
//

#include "GUI.hpp"
#include "GUICore.hpp"
namespace kege{

    GUIElem* GUICore::operator()( const kege::MsgMousePosition& msg, GUIElem* elem, const kege::vec4& transform )
    {
        if ( elem->_flags & GUIElem::INACTIVE )
            return nullptr;

        GUIElem* ui = nullptr;
        if ( elem->children() != nullptr )
        {
            for (auto& child : *elem->children() )
            {
                ui = operator()( msg, child._elem, child._transform );
                if ( ui != nullptr )
                {
                    return ui;
                }
            }
        }

        bool mouse_enter =
        (
         transform.x <= msg._x && msg._x <= transform.x + elem->_width &&
         transform.x <= msg._y && msg._y <= transform.y + elem->_height
        );

        if ( mouse_enter )
        {
            if ( !(elem->_state & GUIElem::MOUSE_ENTER) )
            {
                elem->_state |= GUIElem::MOUSE_ENTER;
                onEvent( elem, GUIEvent::MOUSE_ENTER );
                return elem;
            }
        }

        if ( !(elem->_state & GUIElem::MOUSE_ENTER) )
        {
            elem->_state &= ~GUIElem::MOUSE_ENTER;
            onEvent( elem, GUIEvent::MOUSE_EXIT );
        }
        return nullptr;
    }

    void GUICore::onMessage( const kege::Ref< kege::Message >& msg )
    {
        if ( msg->_type == kege::Message::USER_INPUT )
        {
            std::lock_guard< std::mutex > lock( _msg_mutex );
            kege::Ref< kege::Message > m = msg;
            _messages.push( (kege::MsgUserInput*) m.ref() );
        }
    }

    void GUICore::operator()( const kege::MsgMousePosition& msg )
    {
        GUIElem* ui = nullptr;
        for (auto& object : _objects )
        {
            ui = operator()( msg, &object->_nodes, object->_nodes._transform );
            if ( ui != _entered_object )
            {
                _entered_object = ui;
            }
        }
    }

    void GUICore::operator()( const kege::MsgScrollOffset& msg )
    {}

    void GUICore::operator()( const kege::MsgMouseKey& msg )
    {
        if ( msg._key == 0 && _button_down == (msg._state != 0) )
        {
            return;
        }

        _button_down = msg._state != 0;

        if ( !_entered_object )
            return;
        
        if ( _button_down )
        {
            if ( _entered_object->_state & (GUIElem::SELECTABLE | GUIElem::MOUSE_ENTER) )
            {
                _entered_object->_state |= GUIElem::PRESSING;
                onEvent( _entered_object, GUIEvent::PRESSING );
                _curr_selected = _entered_object;
            }
        }
        else if ( _entered_object->_state & (GUIElem::SELECTABLE | GUIElem::PRESSING) )
        {
            _entered_object->_state |= GUIElem::SELECTED;
            _entered_object->_state &= ~GUIElem::PRESSING;
            onEvent( _entered_object, GUIEvent::SELECTED );
            _entered_object = nullptr;
        }
        else if ( _entered_object->_state | GUIElem::SELECTED )
        {
            _entered_object->_state &= ~GUIElem::SELECTED;
            onEvent( _entered_object, GUIEvent::UNSELECTED );
        }
        else if ( _entered_object->_state | GUIElem::PRESSING )
        {
            _entered_object->_state &= ~GUIElem::PRESSING;
            onEvent( _entered_object, GUIEvent::PRESSING );
        }
    }

    void GUICore::operator()( const kege::MsgUserInput& msg )
    {
        switch ( msg._type )
        {
        case kege::MsgUserInput::MOUSE_KEY:
            operator()( (const kege::MsgMouseKey&) msg );
            break;

        case kege::MsgUserInput::MOUSE_POSITION:
            operator()( (const kege::MsgMousePosition&) msg );
            break;

        case kege::MsgUserInput::MOUSE_SCROLL:
            operator()( (const kege::MsgScrollOffset&) msg );
            break;

        default: break;
        }
    }


    void GUICore::dispatchMessages()
    {
        std::queue< kege::Ref< kege::MsgUserInput > > messages;
        {
            std::lock_guard< std::mutex > lock( _msg_mutex );
            messages = _messages;
            _messages = {};
        }

        while ( messages.empty() )
        {
            kege::Ref< kege::MsgUserInput >& msg = messages.front();
            operator()( *msg );
            messages.pop();
        }
    }
}
