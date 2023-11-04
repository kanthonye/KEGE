//
//  Texture.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/13/23.
//

#ifndef Texture_hpp
#define Texture_hpp

#include "../../device/core/Abstraction.h"

namespace kege{namespace vk{
    class Texture;
}}

namespace kege{namespace core{

    enum ImgFormat
    {
        R8_UNORM,
        RG8_UNORM,
        RGB8_UNORM,
        RGBA8_UNORM,

        R8_UINT,
        RG8_UINT,
        RGB8_UINT,
        RGBA8_UINT,

        R16_UINT,
        RG16_UINT,
        RGB16_UINT,
        RGBA16_UINT,

        R32_UINT,
        RG32_UINT,
        RGB32_UINT,
        RGBA32_UINT,

        R64_UINT,
        RG64_UINT,
        RGB64_UINT,
        RGBA64_UINT,

        R8_SINT,
        RG8_SINT,
        RGB8_SINT,
        RGBA8_SINT,

        R16_SINT,
        RG16_SINT,
        RGB16_SINT,
        RGBA16_SINT,

        R32_SINT,
        RG32_SINT,
        RGB32_SINT,
        RGBA32_SINT,

        R64_SINT,
        RG64_SINT,
        RGB64_SINT,
        RGBA64_SINT,

        R32_FLOAT,
        RG32_FLOAT,
        RGB32_FLOAT,
        RGBA32_FLOAT,

        R64_FLOAT,
        RG64_FLOAT,
        RGB64_FLOAT,
        RGBA64_FLOAT,

        R8_SRGB,
        RG8_SRGB,
        RGB8_SRGB,
        RGBA8_SRGB,

        BGR8_SRGB,
        BGRA8_SRGB,

        D16,
        D24,
        D32,
    };

    class Texture : public kege::RefCounter
    {
    public:

        virtual const vk::Texture* vulkan()const = 0;
        virtual uint32_t height()const = 0;
        virtual uint32_t width()const = 0;
        virtual uint32_t depth()const = 0;
        virtual void destruct() = 0;

        KEGE_CORE_ABSTRACTION( Texture );
    };
}}

#endif /* Texture_hpp */
