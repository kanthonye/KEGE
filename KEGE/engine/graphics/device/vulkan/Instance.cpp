//
//  Instance.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/21/23.
//

#include "Instance.hpp"
namespace kege{namespace vk{

    VkResult Instance::construct( const VkDebugUtilsMessengerCreateInfoEXT* msg_create_info, const std::vector< const char* >* layers )
    {
        auto extensions = vk::getRequiredExtensions();
        extensions.emplace_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "KEGE";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Kenneth Esdaile Game Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &appInfo;
        create_info.enabledExtensionCount = static_cast<uint32_t>( extensions.size() );
        create_info.ppEnabledExtensionNames = extensions.data();
        create_info.enabledExtensionCount = (uint32_t) extensions.size();
        create_info.ppEnabledExtensionNames = extensions.data();
        create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

        if ( layers )
        {
            create_info.enabledLayerCount = static_cast<uint32_t>( layers->size() );
            create_info.ppEnabledLayerNames = layers->data();
        }
        else
        {
            create_info.enabledLayerCount = 0;
        }

        if ( msg_create_info )
        {
            create_info.pNext = msg_create_info;
        }
        else
        {
            create_info.pNext = nullptr;
        }

        return vkCreateInstance( &create_info, nullptr, &_instance );
    }

    const VkInstance Instance::handle()const
    {
        return _instance;
    }

    void Instance::destruct()
    {
        if ( _instance != VK_NULL_HANDLE )
        {
            vkDestroyInstance( _instance, nullptr );
            _instance = VK_NULL_HANDLE;
        }
    }

    Instance::~Instance()
    {
        destruct();
    }

    Instance::Instance()
    :   _instance( VK_NULL_HANDLE )
    {}

}}
