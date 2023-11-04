//
//  ParticleShader.cpp
//  dev0
//
//  Created by Kenneth A Esdaile on 5/30/18.
//  Copyright © 2018 Kenneth Anthony Esdaile. All rights reserved.
//
#include "Window.hpp"
#include "ParticleShader.hpp"
namespace kege{namespace gfx{
    
    ParticleShader::ParticleShader()
    {
        if ( !glsl::ShaderProgram::Create("~/shaders/particle-shader.shdr") )
            return;
        Enable();
        
        AddAttribute("vp");
        AddAttribute("translation");
        AddAttribute("dimension");
        AddAttribute("color");
        AddAttribute("frame1");
        AddAttribute("frame2");
        
        int countActiveSU;
        int len, numCompS;
        char name[256];
        
        glGetProgramStageiv
        (  program_handle
         , GL_FRAGMENT_SHADER
         , GL_ACTIVE_SUBROUTINE_UNIFORMS
         , &countActiveSU );
        
        for (int i = 0; i < countActiveSU; ++i)
        {
            glGetActiveSubroutineUniformName
            (  program_handle
             , GL_FRAGMENT_SHADER
             , i
             , 256
             , &len
             , name );
            
            glGetActiveSubroutineUniformiv
            (  program_handle
             , GL_FRAGMENT_SHADER
             , i
             , GL_NUM_COMPATIBLE_SUBROUTINES
             , &numCompS);
            
            int subroutines[numCompS];
            glGetActiveSubroutineUniformiv
            (  program_handle
             , GL_FRAGMENT_SHADER
             , i
             , GL_COMPATIBLE_SUBROUTINES
             , subroutines  );
            
            for (int j=0; j < numCompS; ++j)
            {
                glGetActiveSubroutineName
                (  program_handle
                 , GL_FRAGMENT_SHADER
                 , subroutines[j]
                 , 256, &len, name );
                
                glsl::ShaderStruct::AddSubroutineIndex
                (  GL_FRAGMENT_SHADER
                 , subroutines[j]
                 , name );
            }
        }
        
        SetUniformBlockBinding
        (  GetUniformBlockIndex("CAMERA")
         , glsl::BindingPoint::GetBindingPoint("CAMERA")  );
        
        SetUniformBlockBinding
        (  GetUniformBlockIndex("ATMOSPHERE")
         , glsl::BindingPoint::GetBindingPoint("ATMOSPHERE")  );
    
        Disable();
    }
}}
