//
//  Swapchain.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/9/23.
//

#ifndef Swapchain_hpp
#define Swapchain_hpp
#include <vector>
#include "../../device/vulkan/Framebuffer.hpp"
namespace kege{namespace vk{
    class CommandBuffer;
    class Window;
    class Device;

    class Swapchain
    {
    public:

        const vk::Framebuffer* frameBuffer( uint32_t index )const;
        const VkSwapchainKHR handle()const;

        VkResult construct();
        void destruct();

        ~Swapchain();
        Swapchain();

    private:

        std::vector< vk::Framebuffer > _framebuffers;
        VkSwapchainKHR _swapchain;
    };
}}
#endif /* Swapchain_hpp */
