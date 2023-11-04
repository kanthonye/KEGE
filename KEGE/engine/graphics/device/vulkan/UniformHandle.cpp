//
//  ShaderResource.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/4/23.
//

#include "../../device/vulkan/Device.hpp"
#include "../../device/vulkan/Descriptor.hpp"
#include "../../device/vulkan/UniformHandle.hpp"

namespace kege{namespace vk{


    const std::set< vk::PipelineLayout* >* UniformHandle::getPipelineLayoutSet()const
    {
        if ( _descriptor_pool )
        {
            return _descriptor_pool->allocator()->descriptorSetLayout()->pipelineLayoutSet();
        }
        return nullptr;
    }

    const VkDescriptorSet UniformHandle::descriptorSet()const
    {
        return _descriptor_set;
    }

    void UniformHandle::update( core::UniformLayout* asset )
    {
        vk::Device::get()->getPipelineLayoutManager()->updateUniformAsset( this, asset );
    }

    const vk::UniformHandle* UniformHandle::vulkan()const
    {
        return this;
    }

    vk::UniformHandle* UniformHandle::vulkan()
    {
        return this;
    }

    void UniformHandle::destruct()
    {
        if ( _descriptor_pool )
        {
            _descriptor_pool->release( _descriptor_set );
            _descriptor_pool = nullptr;
        }
    }

    UniformHandle::~UniformHandle()
    {
        destruct();
    }

    UniformHandle::UniformHandle( VkDescriptorSet descriptor, vk::DescriptorPool* pool )
    :   _descriptor_pool( pool )
    ,   _descriptor_set( descriptor )
    {}
}}
