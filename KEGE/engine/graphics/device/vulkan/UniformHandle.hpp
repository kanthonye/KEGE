//
//  ShaderResource.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/4/23.
//

#ifndef KEGE_VULKAN_UNIFORM_HANDLE_HPP
#define KEGE_VULKAN_UNIFORM_HANDLE_HPP

#include "../../device/vulkan/utils.hpp"

namespace kege{namespace vk{

    class UniformHandle : public core::UniformHandle
    {
    public:

        const std::set< vk::PipelineLayout* >* getPipelineLayoutSet()const;

        void setDescriptorSet( VkDescriptorSet descriptor );
        const VkDescriptorSet descriptorSet()const;

        void update( core::UniformLayout* layout );

        const vk::UniformHandle* vulkan()const;
        vk::UniformHandle* vulkan();

        void destruct();

        UniformHandle( VkDescriptorSet descriptor, vk::DescriptorPool* pool );
        ~UniformHandle();

    private:

        vk::DescriptorPool* _descriptor_pool;
        VkDescriptorSet _descriptor_set;
        friend vk::PipelineLayoutManager;
    };

}}
#endif /* KEGE_VULKAN_UNIFORM_HANDLE_HPP */
