//
//  EmissionTimer.hpp
//  dev0
//
//  Created by Kenneth A Esdaile on 9/12/18.
//  Copyright © 2018 Kenneth Anthony Esdaile. All rights reserved.
//

#ifndef EmissionTimer_hpp
#define EmissionTimer_hpp
#include "Particle.hpp"
#include "Component.hpp"
namespace kege{namespace efx{
    
    /**
     * EmissionTimer is used to tell ParticleEmitter when to emit a particle.
     */
    class EmissionTimer : public scene::Component
    {
    public:
        
        void SetEmissionRate(unsigned short sec, unsigned short count);
        void Update(scene::Gameobject& obj, float dt);
        
        float GetEmissionTime()const;
        int GetEmissionSize()const;
        
        EmissionTimer(unsigned short sec, unsigned short count);
        ~ EmissionTimer();
        EmissionTimer();
        
    private:
        
        scene::MsgInt msg;
        float emission_time;
        float emission_rate;
        float emission_size;
        float timespand;
    };
}}
#endif /* EmissionTimer_hpp */
