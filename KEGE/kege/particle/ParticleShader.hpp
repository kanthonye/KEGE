//
//  ParticleShader.hpp
//  dev0
//
//  Created by Kenneth A Esdaile on 5/30/18.
//  Copyright © 2018 Kenneth Anthony Esdaile. All rights reserved.
//

#ifndef ParticleShader_hpp
#define ParticleShader_hpp
#include "ShaderProgram.hpp"
namespace kege{namespace gfx{
    
    class ParticleShader : public glsl::ShaderProgram
    {
    public:
        static ParticleShader& GetInstance()
        {
            static ParticleShader shader;
            return shader;
        }
        ParticleShader();
    };
}}
#endif /* ParticleShader_hpp */
