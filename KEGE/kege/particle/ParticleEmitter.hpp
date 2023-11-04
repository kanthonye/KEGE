//
//  ParticleEmitter.hpp
//  AppDev
//
//  Created by Kenneth Esdaile on 5/16/16.
//  Copyright © 2016 Kenneth Anthony Esdaile. All rights reserved.
//

#ifndef ParticleEmitter_hpp
#define ParticleEmitter_hpp
#include "mempool.hpp"
#include "Particle.hpp"
#include "Component.hpp"
#include "ParticleSystem.hpp"
namespace kege{namespace efx{
    
    /**
     * ParticleEmitter handles how the particles are spawn. Different emitters
     * can spawn particles in different ways. for instance partices can be spawn
     * from a point, on a line, in a circular line, or sphere, cube, cone etc.
     * This is what the ParticleEmitter is used for.
     */
    class ParticleEmitter : public scene::Component
    {
    public:
        
        bool ReceiveMessage(scene::Gameobject& obj, const scene::Message& message);
        void CreateParticles(scene::Gameobject& obj, cg::ushort total);
        cg::point3 GetSpawnPoint(scene::Gameobject& obj);
        
        ~ ParticleEmitter();
        ParticleEmitter();
        
    private:
        
        efx::ParticleSystem* particle_system;
    };
}}
#endif /* ParticleEmitter_hpp */
