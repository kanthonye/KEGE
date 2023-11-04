//
//  Material.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/4/23.
//

#ifndef KEGE_MATERIAL_HPP
#define KEGE_MATERIAL_HPP
#include <vector>
#include "../../memory/Ref.hpp"
#include "../material/ShaderEffect.hpp"
namespace kege
{
    typedef std::vector< kege::Ref< kege::ShaderEffect > > ShaderEffects;

    class Material : public kege::RefCounter
    {
    public:

        Material( const kege::ShaderEffects& effects );

        const kege::ShaderEffects& effects()const;
        kege::ShaderEffects& effects();
        
        ~Material();
        Material();

    public:

        kege::ShaderEffects _effects;
	};
}
#endif /* KEGE_MATERIAL_HPP */
