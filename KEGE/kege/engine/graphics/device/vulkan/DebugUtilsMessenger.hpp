//
//  ValidationLayer.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/21/23.
//

#ifndef ValidationLayer_hpp
#define ValidationLayer_hpp

#include "../vulkan/utils.hpp"

namespace kege{namespace vk{

    class DebugUtilsMessenger : public kege::RefCounter
    {
    public:

        const VkDebugUtilsMessengerCreateInfoEXT& info()const;
        VkResult construct( const Instance& instance );
        void destruct();
        const VkDebugUtilsMessengerEXT handle()const;
        const kege::string& sid()const;
        ~DebugUtilsMessenger();
        DebugUtilsMessenger();

    private:

        VkDebugUtilsMessengerCreateInfoEXT _info;
        VkDebugUtilsMessengerEXT _handle;
        const vk::Instance* _instance;
        kege::string _sid;
    };

}}
#endif /* ValidationLayer_hpp */
