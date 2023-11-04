//
//  ShaderLibrary.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/17/23.
//

#ifndef ShaderLibrary_hpp
#define ShaderLibrary_hpp

#include "ResMgr.hpp"
#include "Hashmap.hpp"
#include "../graphics/device/core/Shader.hpp"
namespace kege
{
    class ShaderLibrary
    {
    public:

        kege::Ref< core::Shader > load( const kege::string& filename );
        kege::Ref< core::Shader > get( const kege::string& name );
        void uninit();
        bool init();

        ~ShaderLibrary();
        
    private:

        kege::ResourceT< Ref< core::Shader > > _shaders;
        kege::Hashmap< kege::string > _filepaths;
    };
}
#endif /* ShaderLibrary_hpp */
