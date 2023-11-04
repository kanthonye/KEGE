//
//  Input.hpp
//  kege-rendering-engine
//
//  Created by Kenneth A Esdaile on 3/9/22.
//

#ifndef Input_hpp
#define Input_hpp

#include "glm.hpp"
#include "Keycodes.hpp"
#include "Message.hpp"
#include "../../ecs/EntitySystem.hpp"

namespace kege{namespace system{

    /**
     The input system's purpose is to convert human input into events. 
     events are integer values. these integer values are used to make 
     function callbacks via the CallbackManager.
     */
    class Input : public kege::EntitySystem
    {
    public:
        
        //bool init( kege::EntityManager* emgr, kege::Messenger* msgr, kege::ResMgr* rmgr, kege::SystemsManager* smgr );
        void operator()( const kege::Ref< Message >& msg );
        //void receive( const kege::Ref< kege::Message >& message );
        //void mousePosition( const kege::dvec2& position );
        //void mouseScroll( const kege::dvec2& position );
        ///void keyboardKey( const kege::KeyboardKey& key );
        //void mouseKey( const kege::MouseKey& key );
        //void uninit();
        
        void toggleWireframe( const int& event, const float& entity );
        
        void setPlayer( const int& entity );

        bool bind( const int32_t& key, const int32_t& action, bool toggle = false );
        void update( const int32_t& key, const double& state );
        void update( const double& dms );
        void updateMousePosition();
        void updateMouseScroll();

        Input( int priority );
        ~Input();
        
    private:

        std::vector< kege::Ref< const MsgUserInput > > _message;
        //kege::Ref< kege::KeybindManager > _kmgr;
//
        typedef int32_t key;
        typedef int32_t action;

        class Binds
        {
        public:

            int32_t _action;
            bool _toggle;
            double _states[2];
        };

        std::unordered_map< key, Binds* > _keybinds;
        std::unordered_map< action, Binds* > _actionbinds;
        std::vector< Binds* > _activebinds;
        
        kege::dvec2 _mouse_scroll;
        kege::dvec2 _mouse_position;
        kege::dvec2 _mouse_prevpos;
        bool _pointer_dir_states[ 4 ];
        bool _scroll_dir_states[ 4 ];
        int _player;
    };
}}

#include "event_enums.hpp"

namespace kege
{
    struct InputControl : public kege::RefCounter
    {
        virtual void set( uint16_t command, float value ) = 0;
        virtual float get( uint16_t command )const = 0;
        virtual ~InputControl(){};
    };
}

namespace kege
{
    struct PlayerInput : public kege::InputControl
    {
        PlayerInput(){ memset(_states, 0x0, 64*sizeof(float) ); }
        void set( uint16_t command, float value ){ _states[ command ] = value; }
        float get( uint16_t command )const{ return _states[ command ]; }
        float _states[ 64 ];
    };
}


namespace kege{ namespace component{

    typedef kege::Ref< InputControl > Input;
    
}}
#endif /* Input_hpp */
