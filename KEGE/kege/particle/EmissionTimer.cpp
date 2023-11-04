//
//  EmissionTimer.cpp
//  dev0
//
//  Created by Kenneth A Esdaile on 9/12/18.
//  Copyright © 2018 Kenneth Anthony Esdaile. All rights reserved.
//
#include "Gameobject.hpp"
#include "EmissionTimer.hpp"
#define  MAX_PARTICLE_SIZE 4096
namespace kege{namespace efx{
    
    void efx::EmissionTimer::SetEmissionRate(unsigned short sec, unsigned short count)
    {
        emission_time = sec;
        emission_size = count;
        emission_rate = (1000.0 * emission_time) / (float)emission_size;
    }
    
    void efx::EmissionTimer::Update(scene::Gameobject& obj, float delta_ms)
    {
        timespand += delta_ms;
        int total = timespand / emission_rate;
        if (total >= 1)
        {
            msg.value = cg::math::min(total, 4096);
            obj.MessageComponents( msg );
            timespand -= total * emission_rate;
        }
    }
    
    float efx::EmissionTimer::GetEmissionTime()const
    {
        return emission_time;
    }
    
    int efx::EmissionTimer::GetEmissionSize()const
    {
        return emission_size;
    }
    
    EmissionTimer::EmissionTimer(unsigned short sec, unsigned short count)
    :   scene::Component( scene::GetTypID<efx::EmissionTimer>() )
    ,   msg(scene::MsgInt(GetTypeID(), this, 0,"emit"))
    ,   emission_rate(0)
    ,   emission_time(1)
    ,   emission_size(1)
    ,   timespand(0)
    {
        SetEmissionRate(sec, count);
    }
    
    EmissionTimer::EmissionTimer()
    :   scene::Component( scene::GetTypID<efx::EmissionTimer>() )
    ,   msg(scene::MsgInt(GetTypeID(), this, 0,"emit"))
    ,   emission_rate(0)
    ,   emission_time(1)
    ,   emission_size(1)
    ,   timespand(0)
    {
        SetEmissionRate(1, 1);
    }
    
    EmissionTimer::~ EmissionTimer()
    {
    }
}}
