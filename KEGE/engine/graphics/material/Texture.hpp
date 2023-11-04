//
//  Texture.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/7/23.
//

#ifndef KEGE_CORE_TEXTURE
#define KEGE_CORE_TEXTURE
#include <stdlib.h>
//#include "../../../../memory/pool-system/PoolSysm.hpp"
//#include "../../device/core/Image.hpp"
//namespace kege{namespace vk{
//    class Image;
//    class Sampler;
//    class Device;
//}}
//
//namespace kege{namespace core{
//
////    typedef enum SamplerFilter {
////        NEAREST,
////        LINEAR,
////    } SamplerFilter;
////
////    typedef enum SamplerWrapping {
////        REPEAT,
////        MIRRORED_REPEAT,
////        CLAMP_TO_EDGE,
////        CLAMP_TO_BORDER,
////        MIRROR_CLAMP_TO_EDGE,
////        EDGE_MODE_MAX_ENUM
////    } SamplerWrapping;
////
////    typedef enum SamplerMipmap {
////        MIPMAP_MODE_NEAREST,
////        MIPMAP_MODE_LINEAR,
////        MIPMAP_MODE_MAX_ENUM
////    } SamplerMipmap;
////
////    struct Texture2dCreateInfo
////    {
////        uint32_t width;
////        uint32_t height;
////        uint32_t channels;
////        const void* data;
////        SamplerFilter min = kege::core::LINEAR;
////        SamplerFilter mag = kege::core::LINEAR;
////        SamplerWrapping wrap_x = kege::core::CLAMP_TO_EDGE;
////        SamplerWrapping wrap_y = kege::core::CLAMP_TO_EDGE;
////    };
////
////    struct CreateTexture2dInfo
////    {
////        uint32_t _width;
////        uint32_t _height;\
////        uint32_t _format;
////        const void* _data;
////    };
////
////    struct CreateSampler2dInfo
////    {
////        uint32_t _min_filter;
////        uint32_t _mag_filter;
////        uint32_t _wrap_x;
////        uint32_t _wrap_y;
////    };
//
//
//
//    
//    class Texture
//    {
//    public:
//
//        virtual const vk::Image* vulkan()const{ return nullptr; }
//        virtual vk::Image* vulkan(){ return nullptr; }
//
//        virtual const core::Sampler* sampler()const = 0;
//        virtual const core::Image* image()const = 0;
//        virtual core::Sampler* sampler() = 0;
//        virtual core::Image* image() = 0;
//
//        KEGE_CORE_ABSTRACTION( Texture );
//    };
//
//
//
//}}
#endif /* KEGE_CORE_TEXTURE */
