//
//  ParticleRenderer.hpp
//  dev0
//
//  Created by Kenneth A Esdaile on 5/30/18.
//  Copyright © 2018 Kenneth Anthony Esdaile. All rights reserved.
//

#ifndef ParticleRenderer_hpp
#define ParticleRenderer_hpp
#include "Renderer.hpp"
namespace kege {namespace gfx{
    
    class ParticleRenderer : public gfx::Renderer
    {
    public:
        
        bool Draw
        (  const cg::point3& position
         , int width, int height
         , int texID, float blend
         , const cg::vec4& colr
         , const cg::vec4& frame1
         , const cg::vec4& frame2  );
        
        bool Draw
        (  const cg::point3& position
         , int width, int height, int texID
         , const cg::vec4& colr
         , const cg::vec4& frame1  );
        
        bool Draw
        (  const cg::point3& position
         , int width, int height
         , const cg::vec4& colr  );
        
        void Render(const scene::Model& model){}
        void Render(const glsl::ShaderProgram* shader_program);
        void Flush();
        
        ~ ParticleRenderer();
        ParticleRenderer();
        
        static ParticleRenderer& GetInstance();
        
    private:
        
        struct GraphicsElement
        {
            cg::point4 position;  // xyz = position, w = scale
            cg::point4 dimension; // x = width, y = height, z=texID, w=blend,
            cg::vec4 color; // x = width, y = height
            cg::vec4 frame1; // xy = tile_pos, zw = unit width and height,
            cg::vec4 frame2; // xy = tile_pos, zw = unit width and height,
        };
        
    private:
        
        ds::array<GraphicsElement> graphics_buffer;
        unsigned int buffer_stride;
        unsigned int drawcount;
        gfx::VAO vao;
        gfx::VBO vbo;
        bool init;
    };
}}
#endif /* ParticleRenderer_hpp */
