//
//  ParticleEffect.hpp
//  AppDev
//
//  Created by Kenneth Esdaile on 5/16/16.
//  Copyright © 2016 Kenneth Anthony Esdaile. All rights reserved.
//

#ifndef ParticleEffect_hpp
#define ParticleEffect_hpp
#include "array.h"
#include "Force.hpp"
#include "Particle.hpp"
#include "Component.hpp"
namespace kege{namespace efx{
    
    /**
     * ParticleSystem manages particles that are added to it.
     * the managing includes updates by applying forces to particles,
     * updating there orientation etc, and removing inactive particles.
     */
    class ParticleSystem : public scene::Component
    {
    public:
        
        void Render(scene::Gameobject& obj, const gfx::RenderPass& pass, const scene::Frustum& frustum);
        bool ReceiveMessage(scene::Gameobject& obj, const scene::Message& message);
        void Update(scene::Gameobject& owner, float delta_ms);
        void SimulateCollisions(float delta_ms);
        void SimulateParticles(float delta_ms);
        
        efx::Particle* SpawnParticle(scene::Gameobject& owner);
        
        void AddForce(efx::Force* force);
        
        void SetSaturationGen(cg::rand1f const& gen);
        cg::rand1f const& GetSaturationGen()const;
        cg::rand1f & GetSaturationGen();
        
        void SetVelocityGen(cg::rand3f const& gen);
        cg::rand3f  const& GetVelocityGen()const;
        cg::rand3f  & GetVelocityGen();
        
        void SetSpeedGen(cg::rand1f const& gen);
        cg::rand1f const& GetSpeedGen()const;
        cg::rand1f & GetSpeedGen();
        
        void SetColorGen(cg::rand4f const& gen);
        cg::rand4f  const& GetColorGen()const;
        cg::rand4f  & GetColorGen();
        
        void SetMassGen(cg::rand1f const& gen);
        cg::rand1f const& GetMassGen()const;
        cg::rand1f & GetMassGen();
        
        void SetSizeGen(cg::rand1f const& gen);
        cg::rand1f const& GetSizeGen()const;
        cg::rand1f & GetSizeGen();
        
        void SetLifeGen(cg::rand1f const& gen);
        cg::rand1f const& GetLifeGen()const;
        cg::rand1f & GetLifeGen();
        void Destroy();
        
        virtual ~ ParticleSystem();
        ParticleSystem();
        
    protected:
        
        ds::array<efx::Particle*> particles;
        ds::array<efx::Force*> forces;
        
        cg::vec3 owner_velocity;
        cg::vec3 owner_position;
        
        cg::rand1f saturation;
        cg::rand3f velocity;
        cg::rand4f color;
        cg::rand1f speed;
        cg::rand1f mass;
        cg::rand1f size;
        cg::rand1f life;
        float decay;
    };

    class MsgForce : public scene::Message
    {
    public:
        
        // Anonymous messaging
        MsgForce(efx::Force* force, const ds::string& msg)
        :   scene::Message(scene::GenTypID<MsgForce>(), msg)
        ,   force(force)
        {}
        
        efx::Force* force;
    };
}}
#endif /* ParticleEffect_hpp */
