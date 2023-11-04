//
//  Sampler.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/5/23.
//

#ifndef KEGE_CORE_SAMPLER_HPP
#define KEGE_CORE_SAMPLER_HPP

#include "../../device/core/Abstraction.h"

namespace kege{namespace vk{
    class Sampler;
}}

namespace kege{namespace core{

    class Sampler : public kege::RefCounter
    {
    public:

        typedef enum Filter {
            NEAREST,
            LINEAR,
            CUBIC,
        } Filter;

        typedef enum Wrapping {
            REPEAT,
            MIRRORED_REPEAT,
            CLAMP_TO_EDGE,
            CLAMP_TO_BORDER,
            MIRROR_CLAMP_TO_EDGE,
            EDGE_MODE_MAX_ENUM
        } Wrapping;

        typedef enum MipmapFilter {
            MIPMAP_NEAREST,
            MIPMAP_LINEAR,
            MIPMAP_MAX_ENUM
        } MipmapFilter;

        virtual const vk::Sampler* vulkan()const{return nullptr;}
        virtual vk::Sampler* vulkan(){return nullptr;}

        virtual void destruct() = 0;

        KEGE_CORE_ABSTRACTION( Sampler );
    };

}}
#endif /* KEGE_CORE_IMAGE_HPP */
