//
//  Sampler.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/21/23.
//

#ifndef KEGE_VULKAN_SAMPLER_HPP
#define KEGE_VULKAN_SAMPLER_HPP

#include "../vulkan/utils.hpp"

namespace kege{namespace vk{

    class Sampler : public kege::core::Sampler
    {
    public:

        VkResult construct
        (
            core::Sampler::Filter min,
            core::Sampler::Filter mag,
            core::Sampler::Wrapping x,
            core::Sampler::Wrapping y,
            core::Sampler::Wrapping z
        );


        const vk::Sampler* vulkan()const{return this;}
        vk::Sampler* vulkan(){return this;}

        const VkSampler handle()const;
        void destruct();
        ~Sampler();
        Sampler();

    private:

        VkSampler _handle;
    };

}}

#endif /* KEGE_VULKAN_SAMPLER_HPP */
