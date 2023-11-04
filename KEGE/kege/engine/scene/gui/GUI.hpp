//
//  GUI.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/11/23.
//

#ifndef GUI_hpp
#define GUI_hpp

#include "Message.hpp"
#include "GUIElem.hpp"
namespace kege{

    class GUI
    {
    public:

        void onMessage( const kege::Ref< kege::Message >& msg );
        void operator()( const kege::MsgMousePosition& msg );
        void operator()( const kege::MsgScrollOffset& msg );
        void operator()( const kege::MsgMouseKey& msg );
        void operator()( const kege::MsgUserInput& msg );

        GUINode _nodes;
    };
}
#endif /* GUI_hpp */
