//
//  Framebuffer.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 6/11/23.
//

#ifndef KEGE_CORE_FRAMEBUFFER
#define KEGE_CORE_FRAMEBUFFER
#include <stdlib.h>
#include "../core/Texture.hpp"
namespace kege{namespace vk{
    class Framebuffer;
}}
namespace kege{namespace core{

    enum FBOCompType
    {
        COLOR_COMPONENT,
        DEPTH_COMPONENT,
    };

    class Framebuffer : public kege::RefCounter
    {
    public:

        virtual void attach( FBOCompType type, const kege::Ref< core::Texture >& texture ) = 0;

        virtual const vk::Framebuffer* vulkan()const = 0;
        virtual vk::Framebuffer* vulkan() = 0;
        virtual uint32_t height()const = 0;
        virtual uint32_t width()const = 0;
        virtual bool construct() = 0;
        virtual void destruct() = 0;

        virtual ~Framebuffer(){}
    };
}}
#endif /* KEGE_CORE_FRAMEBUFFER */
