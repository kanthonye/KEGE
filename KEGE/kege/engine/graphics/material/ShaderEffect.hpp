//
//  ShaderEffect.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/12/23.
//

#ifndef KEGE_SHADER_EFFECT_HPP
#define KEGE_SHADER_EFFECT_HPP
#include "Ref.hpp"
#include "../device/core/Shader.hpp"
#include "../device/core/UniformLayout.hpp"
namespace kege
{
    class ShaderEffect : public core::UniformLayout
    {
    public:

        const core::Shader* shader()const;
        core::Shader* shader();

        virtual ~ShaderEffect();
        ShaderEffect();

    protected:

        Ref< core::Shader > _shader;
    };
}
#endif /* KEGE_SHADER_EFFECT_HPP */
