//
//  ParticleRenderer.cpp
//  dev0
//
//  Created by Kenneth A Esdaile on 5/30/18.
//  Copyright © 2018 Kenneth Anthony Esdaile. All rights reserved.
//

#include "ParticleRenderer.hpp"
#define MAX_INSTANCE_COUNT 10000
namespace kege {namespace gfx{

    ParticleRenderer& gfx::ParticleRenderer::GetInstance()
    {
        static ParticleRenderer instance;
        return instance;
    }
    
    bool gfx::ParticleRenderer::Draw
    (  const cg::point3& position
     , int width, int height
     , int texID, float blend
     , const cg::vec4& colr
     , const cg::vec4& frame1
     , const cg::vec4& frame2  )
    {
        if (drawcount >= MAX_INSTANCE_COUNT) return false;
        
        graphics_buffer[drawcount].position.operator()(position.x,position.y,position.z, 0.0);
        graphics_buffer[drawcount].dimension.operator()(width,height,texID, blend);
        graphics_buffer[drawcount].color = colr;
        graphics_buffer[drawcount].frame1 = frame1;
        graphics_buffer[drawcount].frame2 = frame2;
        drawcount++;
        return true;
    }
    
    bool gfx::ParticleRenderer::Draw
    (  const cg::point3& position
     , int width, int height, int texID
     , const cg::vec4& colr
     , const cg::vec4& frame1  )
    {
        if (drawcount >= MAX_INSTANCE_COUNT) return false;
        
        graphics_buffer[drawcount].position.operator()(position.x,position.y,position.z, 0.0);
        graphics_buffer[drawcount].dimension.operator()(width,height,texID, 0.0);
        graphics_buffer[drawcount].color = colr;
        graphics_buffer[drawcount].frame1 = frame1;
        graphics_buffer[drawcount].frame2 = 0.0f;
        drawcount++;
        return true;
    }
    
    bool gfx::ParticleRenderer::Draw(const cg::point3& position, int width, int height, const cg::vec4& colr)
    {
        if (drawcount >= MAX_INSTANCE_COUNT) return false;
        
        graphics_buffer[drawcount].position.operator()(position.x,position.y,position.z, 0.0);
        graphics_buffer[drawcount].dimension.operator()(width,height,0.0, 0.0);
        graphics_buffer[drawcount].color = colr;
        graphics_buffer[drawcount].frame1 = 0.0f;
        graphics_buffer[drawcount].frame2 = 0.0f;
        drawcount++;
        return true;
    }

    
    void gfx::ParticleRenderer::Render(const glsl::ShaderProgram* shader_program)
    {
        if (drawcount == 0) return;
        
        shader_program->EnableDepthTest();
        shader_program->Enable();
        
        vao.Bind();
        vbo.SubBufferData(0, buffer_stride*drawcount, graphics_buffer.getarray());
        
        vao.DrawInstanced(drawcount);
        vao.Unbind();
        
        shader_program->Disable();
        shader_program->DisableDepthTest();
    }
    
    void gfx::ParticleRenderer::Flush()
    {
        drawcount = 0;
    }

    gfx::ParticleRenderer::ParticleRenderer()
    {
        graphics_buffer.setsize(MAX_INSTANCE_COUNT);
        graphics_buffer.setcount(MAX_INSTANCE_COUNT);
        buffer_stride = sizeof(GraphicsElement);
        drawcount = 0;
        
        graphics_buffer[drawcount].position.operator()(0.0);
        graphics_buffer[drawcount].dimension.operator()(1.0);
        graphics_buffer[drawcount].color(1.0);
        
        // InitializeGeometry
        const long vec4size = sizeof(cg::vec4);
        if ( vao.Create() )
        {
            if (vao.GetVBO().Create())
            {
                cg::vec2 vertice[4];
                vertice[0] = { 0.5f, 0.5f};
                vertice[1] = {-0.5f, 0.5f};
                vertice[2] = { 0.5f,-0.5f};
                vertice[3] = {-0.5f,-0.5f};
                vao.GetVBO().CreateStaticBuffer(vertice, 8*sizeof(float));
                vao.GetVBO().SetVertexAttrib(VERTEX_POSITION, 2, 0, 0);
                vao.GetVBO().Unbind();
            }
            
            const unsigned int LOCATION_TRANSLATION = 1;
            const unsigned int LOCATION_DEMENSION = 2;
            const unsigned int LOCATION_COLOR = 3;
            const unsigned int LOCATION_FRAME1 = 4;
            const unsigned int LOCATION_FRAME2 = 5;
            unsigned int counter = 0;
            
            if (vbo.Create())
            {
                vbo.CreateDynamicBuffer(graphics_buffer.getarray(), buffer_stride*graphics_buffer.count());
                
                vbo.SetVertexAttrib(LOCATION_TRANSLATION, 4, buffer_stride, vec4size*counter++);
                glVertexAttribDivisor(LOCATION_TRANSLATION, 1);
                
                vbo.SetVertexAttrib(LOCATION_DEMENSION, 4, buffer_stride, vec4size*counter++);
                glVertexAttribDivisor(LOCATION_DEMENSION, 1);
                
                vbo.SetVertexAttrib(LOCATION_COLOR, 4, buffer_stride, vec4size*counter++);
                glVertexAttribDivisor(LOCATION_COLOR, 1);
                
                vbo.SetVertexAttrib(LOCATION_FRAME1, 4, buffer_stride, vec4size*counter++);
                glVertexAttribDivisor(LOCATION_FRAME1, 1);
                
                vbo.SetVertexAttrib(LOCATION_FRAME2, 4, buffer_stride, vec4size*counter++);
                glVertexAttribDivisor(LOCATION_FRAME2, 1);
                vbo.Unbind();
            }
            vao.DrawTriangleStrips();
            vao.SetDrawCount(4);
            vao.Unbind();
        }
    }
    
    gfx::ParticleRenderer::~ ParticleRenderer()
    {
        graphics_buffer.clear();
        vao.Destroy();
    }
}}



