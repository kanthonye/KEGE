//
//  ShaderEffect.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/12/23.
//

#include "../material/ShaderEffect.hpp"
namespace kege
{
    const core::Shader* ShaderEffect::shader()const
    {
        return _shader.ref();
    }

    core::Shader* ShaderEffect::shader()
    {
        return _shader.ref();
    }

    ShaderEffect:: ~ShaderEffect()
    {
        //_parameters.clear();
        _shader = nullptr;
    }

    ShaderEffect::ShaderEffect()
    :   _shader( nullptr )
    {}
}
