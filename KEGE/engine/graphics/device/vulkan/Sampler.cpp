//
//  Sampler.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/21/23.
//

#include "../vulkan/Sampler.hpp"
#include "../vulkan/Device.hpp"

namespace kege{namespace vk{

    VkResult Sampler::construct
    (
        core::Sampler::Filter min,
        core::Sampler::Filter mag,
        core::Sampler::Wrapping x,
        core::Sampler::Wrapping y,
        core::Sampler::Wrapping z
    )
    {
        VkSamplerCreateInfo sampler_info{};
        sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

        sampler_info.magFilter = vk::getFilter( min );
        sampler_info.minFilter = vk::getFilter( mag );

        sampler_info.addressModeU = vk::getSamplerAddressMode( x );
        sampler_info.addressModeV = vk::getSamplerAddressMode( y );
        sampler_info.addressModeW = vk::getSamplerAddressMode( z );

        sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        sampler_info.unnormalizedCoordinates = VK_FALSE;

        sampler_info.compareEnable = VK_FALSE;
        sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;

        sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        sampler_info.mipLodBias = 0.0f;
        sampler_info.minLod = 0.0f;
        sampler_info.maxLod = 0.0f;

        sampler_info.anisotropyEnable = VK_FALSE;
        sampler_info.maxAnisotropy = vk::Device::get()->getPhysicalDeviceProperties().limits.maxSamplerAnisotropy;
        /* The maxAnisotropy field limits the amount of texel samples that can be used to calculate the
        final color. A lower value results in better performance, but lower quality results. To
        figure out which value we can use, we need to retrieve the properties of the physical device.
        */
        VkResult result = vkCreateSampler( vk::Device::get()->logicalDevice(), &sampler_info, nullptr, &_handle );
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR( "%s: failed to create texture sampler!", vk::vkGetError( result ) );
            return result;
        }
        return result;
    }

    const VkSampler Sampler::handle()const
    {
        return _handle;
    }

    void Sampler::destruct()
    {
        if ( _handle != VK_NULL_HANDLE )
        {
            vk::Device::get()->waitIdle();
            vkDestroySampler( vk::Device::get()->logicalDevice(), _handle, nullptr );
            _handle = VK_NULL_HANDLE;
        }
    }

    Sampler::~Sampler()
    {
        destruct();
    }

    Sampler::Sampler()
    :   _handle( VK_NULL_HANDLE )
    {}

}}
