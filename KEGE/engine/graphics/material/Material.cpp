//
//  Material.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/4/23.
//

#include "../material/Material.hpp"
namespace kege
{
    Material::Material( const kege::ShaderEffects& effects )
    :   _effects( effects )
    {}

    Material::~Material()
    {
        _effects.clear();
    }

    const kege::ShaderEffects& Material::effects()const
    {
        return _effects;
    }

    kege::ShaderEffects& Material::effects()
    {
        return _effects;
    }
    
}
