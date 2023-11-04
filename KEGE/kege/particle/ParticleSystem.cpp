//
//  ParticleEffect.cpp
//  AppDev
//
//  Created by Kenneth Esdaile on 5/16/16.
//  Copyright © 2016 Kenneth Anthony Esdaile. All rights reserved.
//
#include "Gameobject.hpp"
#include "ParticleRenderer.hpp"
#include "ParticleSystem.hpp"
namespace kege{namespace efx{
    
    void efx::ParticleSystem::Render(scene::Gameobject& obj, const gfx::RenderPass& pass, const scene::Frustum& frustum)
    {
        efx::Particle* particle;
        for (int p=0; p<particles.count(); p++)
        {
            particle = particles[p];
            if (frustum.TestSphere(particle->position, particle->radius))
            {
                gfx::ParticleRenderer::GetInstance().Draw
                (  particle->position
                 , particle->radius
                 , particle->radius
                 , particle->color );
            }
        }
    }
    
    bool efx::ParticleSystem::ReceiveMessage(scene::Gameobject& obj, const scene::Message& message)
    {
        if (message.GetStrMsg() == "add-force")
        {
            if (message.GetMsgType() == scene::GetTypID<efx::MsgForce>())
            {
                AddForce( ((efx::MsgForce&)message).force );
                return true;
            }
        }
        return false;
    }
    
    void efx::ParticleSystem::Update(scene::Gameobject& owner, float delta_ms)
    {
        owner_velocity = owner.GetTransform().GetWorld().GetPosition() - owner_position;
        owner_position = owner.GetTransform().GetWorld().GetPosition();
        SimulateParticles(delta_ms);
        SimulateCollisions(delta_ms);
    }
    
    void ParticleSystem::SimulateParticles(float dt)
    {
        efx::Particle* particle;
        cg::vec3 acceleration(0.0);
        cg::vec3 dv;
        cg::vec3 ds;
        cg::ushort f;
        for (int p=0; p<particles.count(); p++)
        {
            particle = particles[p];
            
            if (particle->life <= 0)
            {
                if (particles.count()-1 != p)
                {
                    particles.swap(p, particles.count()-1);
                }
                particles.setcount(particles.count()-1);
                if (particles.count() >= 1)
                {
                    particle = particles[p];
                }
                else continue;
            }
            
            // apply forces to particles
            for (f=0; f<forces.count(); f++)
            {
                forces[f]->ApplyForce(owner_position, particle);
            }
            
            // compute acceleration from forces acting on particles
            if (particle->collide)
                acceleration = particle->impact_force / particle->mass;
            else
                acceleration = particle->netforce / particle->mass;
            
            // compute velovity from acceleration of particles
            dv = acceleration * dt;
            particle->velocity += dv;
            
            // compute new position of particles
            ds = particle->velocity * dt;
            particle->position += ds;
            
            // compute particle life decay
            particle->life -= dt * decay;
        }
    }
    
    void ParticleSystem::SimulateCollisions(float dt)
    {
        efx::Particle* curr;
        efx::Particle* next;
        cg::vec3 dir;
        cg::vec3 norm;
        float dist;
        float dept;
        cg::ushort total = (cg::ushort)particles.count();
        for (int m,n=0; n<total; n++)
        {
            curr = particles[n];
            curr->impact_force = 0.0;
            curr->impact_force = 0.0;
            
            m = n+1;
            for (; m<total; m++)
            {
                next = particles[m];
                
                dir  = next->position - curr->position;
                dist = cg::magn(dir);
                
                if (curr->radius + next->radius <= dist)
                    continue;
                
                dept = (curr->radius + next->radius) / dist;
                //norm = cg::normalize(dir);
                
                curr->impact_force +=  dir * curr->mass * curr->restitution;
                next->impact_force += -dir * curr->mass * next->restitution;
                curr->collide = true;
                next->collide = true;
            }
        }
    }
    
    void ParticleSystem::AddForce(efx::Force* force)
    {
        forces.append(force);
    }
    
    efx::Particle* ParticleSystem::SpawnParticle(scene::Gameobject& owner)
    {
        efx::Particle * particle = new efx::Particle;
        particles.append(particle);
        
        particle->position = owner.GetTransform().position;
        
        float saturate     = saturation.gen();
        particle->color.x  = color.x.gen() * saturate + (1 - saturate);
        particle->color.y  = color.y.gen() * saturate + (1 - saturate);
        particle->color.z  = color.z.gen() * saturate + (1 - saturate);
        particle->color.w  = color.w.gen();
        particle->radius   = size.gen();
        particle->life     = life.gen();
        particle->mass     = mass.gen();
        
        particle->velocity = velocity.gen() * speed.gen();
        particle->velocity += owner_velocity;
        
        return particle;
    }
    
    void efx::ParticleSystem::SetMassGen(cg::rand1f const& gen)
    {
        mass = gen;
    }
    cg::rand1f const& efx::ParticleSystem::GetMassGen()const
    {
        return mass;
    }
    cg::rand1f & efx::ParticleSystem::GetMassGen()
    {
        return mass;
    }
    
    void efx::ParticleSystem::SetSizeGen(cg::rand1f const& gen)
    {
        size = gen;
    }
    cg::rand1f const& efx::ParticleSystem::GetSizeGen()const
    {
        return size;
    }
    cg::rand1f & efx::ParticleSystem::GetSizeGen()
    {
        return size;
    }
    
    void efx::ParticleSystem::SetLifeGen(cg::rand1f const& gen)
    {
        life = gen;
    }
    cg::rand1f const& efx::ParticleSystem::GetLifeGen()const
    {
        return life;
    }
    cg::rand1f & efx::ParticleSystem::GetLifeGen()
    {
        return life;
    }
    
    void efx::ParticleSystem::SetSaturationGen(cg::rand1f const& gen)
    {
        saturation = gen;
    }
    cg::rand1f const& efx::ParticleSystem::GetSaturationGen()const
    {
        return saturation;
    }
    cg::rand1f & efx::ParticleSystem::GetSaturationGen()
    {
        return saturation;
    }
    
    void efx::ParticleSystem::SetVelocityGen(cg::rand3f const& gen)
    {
        velocity = gen;
    }
    cg::rand3f const& efx::ParticleSystem::GetVelocityGen()const
    {
        return velocity;
    }
    cg::rand3f & efx::ParticleSystem::GetVelocityGen()
    {
        return velocity;
    }
    
    void efx::ParticleSystem::SetSpeedGen(cg::rand1f const& gen)
    {
        speed = gen;
    }
    cg::rand1f const& efx::ParticleSystem::GetSpeedGen()const
    {
        return speed;
    }
    cg::rand1f & efx::ParticleSystem::GetSpeedGen()
    {
        return speed;
    }
    
    void efx::ParticleSystem::SetColorGen(cg::rand4f const& gen)
    {
        color = gen;
    }
    cg::rand4f const& efx::ParticleSystem::GetColorGen()const
    {
        return color;
    }
    cg::rand4f & efx::ParticleSystem::GetColorGen()
    {
        return color;
    }
    void ParticleSystem::Destroy()
    {
        for (int p=0; p<forces.count(); p++)
        {
            delete forces[p];
        }
        forces.clear();
        
        for (int p=0; p<particles.count(); p++)
        {
            delete particles[p];
        }
        particles.clear();
    }
    
    ParticleSystem::~ ParticleSystem()
    {
        Destroy();
    }
    
    ParticleSystem::ParticleSystem()
    :   scene::Component( scene::GetTypID<efx::ParticleSystem>() )
    ,   velocity(cg::rand1f(-0.07f,0.07f), cg::rand1f(0.1f,0.707f), cg::rand1f(-0.07f,0.07f))
    ,   color(cg::rand1f(0.0f,1.0f), cg::rand1f(0.0f,1.0f), cg::rand1f(0.0f,1.0f), cg::rand1f(0.0f,1.0f))
    ,   saturation(0.5f,1.0f)
    ,   speed(0.125, 0.25)
    ,   mass(1.0f, 10.0f)
    ,   life(0.7f, 1.0f)
    ,   size(1.0f, 1.50f)
    ,   decay(0.0016)
    {
    }
    
}}
