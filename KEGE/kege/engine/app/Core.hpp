//
//  Core.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 7/17/23.
//

#ifndef KEGE_Core_hpp
#define KEGE_Core_hpp
#include <queue>
#include <mutex>
#include "clock.hpp"
#include "Message.hpp"
#include "ResMgr.hpp"
#include "VirDir.hpp"
#include "Config.hpp"
#include "../scene/ecs/ECS.hpp"
#include "../utils/resmgr/ResMgr.hpp"
namespace kege{

    class MainLoop;
    class Graphics;
    class Scene;
    class KEGE;

    class Core
    {
    public:

        void receive( MsgRef message );

        kege::Graphics* graphics();
        kege::ResMgr* resources();
        kege::VirDir* virdir();
        kege::Config* config();
        kege::Scene* scene();
        kege::Clock* clock();
        uint32_t fps();

    private:

        Core( const char* app_path );
        void dispatchMessages();
        bool hasMessages()const;
        void uninit();
        bool init();
        ~Core();

    private:

        // message queue for handling window input from the user
        std::queue< kege::Ref< kege::Message > > _messages;
        std::mutex _message_mutex;

        // Graphics rendering visual representation of the current scene
        kege::Graphics* _graphics;

        // Config for loading settings from file
        kege::Config* _config;

        // virtual directory for binding custom directory to real directory
        kege::VirDir* _virdir;

        // resource manager
        kege::ResMgr* _resmgr;
        
        // the current scene
        kege::Scene* _scene;

        kege::Clock _clock;

        // frames-per-second default = 60
        uint32_t _fps;

        // the application path
        kege::string _app_path;

        bool _running;
        
    private:

        friend kege::MainLoop;
        friend kege::KEGE;
    };

}
#endif /* Core_hpp */
