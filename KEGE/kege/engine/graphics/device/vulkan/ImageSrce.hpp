//
//  ImageSrce.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/21/23.
//

#ifndef KEGE_IMAGE_SOURCE_HPP
#define KEGE_IMAGE_SOURCE_HPP

#include "../vulkan/utils.hpp"

namespace kege{namespace vk{

    class ImageSrce
    {
    public:

        VkResult construct( const VkImageCreateInfo& info, VkMemoryPropertyFlags property_bitflags );
        ImageSrce(VkDeviceMemory memory, VkImage handle);
        const VkDeviceMemory memory()const;
        const VkImage handle()const;
        ImageSrce(VkImage handle);
        void destruct();
        ~ImageSrce();
        ImageSrce();

    private:

        VkDeviceMemory _memory;
        VkImage _handle;
    };
}}

#endif /* KEGE_IMAGE_SOURCE_HPP */
