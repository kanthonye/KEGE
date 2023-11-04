//
//  ParticleEmitter.cpp
//  AppDev
//
//  Created by Kenneth Esdaile on 5/16/16.
//  Copyright © 2016 Kenneth Anthony Esdaile. All rights reserved.
//
#include "Gameobject.hpp"
#include "ParticleEmitter.hpp"
#include "ParticleSystem.hpp"
namespace kege{namespace efx{
    
    bool efx::ParticleEmitter::ReceiveMessage(scene::Gameobject& obj, const scene::Message& message)
    {
        if (message.GetStrMsg() == "emit")
        {
            if (message.GetMsgType() == scene::GetTypID<scene::MsgInt>())
            {
                CreateParticles(obj, ((scene::MsgInt&)message).value);
                return true;
            }
        }
        return false;
    }

    void efx::ParticleEmitter::CreateParticles(scene::Gameobject& owner, cg::ushort total)
    {
        if (particle_system == nullptr)
        {
            cg::ushort cmptyp = scene::GetTypID<efx::ParticleSystem>();
            particle_system = (efx::ParticleSystem*) owner.GetComponentManager().Get(cmptyp);
        }
        if (particle_system != nullptr)
        {
            for (int i=0; i<total; i++)
            {
                particle_system->SpawnParticle(owner)->position = GetSpawnPoint(owner);
            }
        }
    }
    
    cg::point3 ParticleEmitter::GetSpawnPoint(scene::Gameobject& obj)
    {
        return obj.GetTransform().position;
    }

    ParticleEmitter::ParticleEmitter()
    :   scene::Component( scene::GetTypID<efx::ParticleEmitter>() )
    ,   particle_system(nullptr)
    {}

    ParticleEmitter::~ ParticleEmitter()
    {
    }
}}
    
    




