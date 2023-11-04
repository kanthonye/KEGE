//
//  GUICore.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/11/23.
//

#ifndef GUICore_hpp
#define GUICore_hpp

#include <queue>
#include <mutex>
#include <vector>
#include "Message.hpp"

namespace kege{

    class GUI;

    class GUICore
    {
    public:

        GUIElem* operator()( const kege::MsgMousePosition& msg, GUIElem* elem, const kege::vec4& transform );
        
        void onMessage( const kege::Ref< kege::Message >& msg );
        void operator()( const kege::MsgMousePosition& msg );
        void operator()( const kege::MsgScrollOffset& msg );
        void operator()( const kege::MsgMouseKey& msg );
        void operator()( const kege::MsgUserInput& msg );

        void onEvent( GUIElem* elem, uint32_t event );
        
        void dispatchMessages();

        std::queue< kege::Ref< kege::MsgUserInput > > _messages;
        std::vector< kege::GUI* > _objects;
        std::mutex _msg_mutex;

        bool _button_down;
        GUIElem* _entered_object;
        GUIElem* _prev_selected;
        GUIElem* _curr_selected;
        GUIElem* _focus_object;
    };
}
#endif /* GUICore_hpp */
