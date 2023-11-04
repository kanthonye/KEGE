//
//  Instance.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/21/23.
//

#ifndef KEGE_VULKAN_INSTANCE
#define KEGE_VULKAN_INSTANCE

#include "../vulkan/utils.hpp"

namespace kege{namespace vk{

    class Instance
    {
    public:

        VkResult construct( const VkDebugUtilsMessengerCreateInfoEXT* msg_create_info, const std::vector< const char* >* layers );
        const VkInstance handle()const;
        void destruct();
        ~Instance();
        Instance();
        
    private:

        VkInstance _instance;
    };

}}
#endif /* KEGE_VULKAN_INSTANCE */
