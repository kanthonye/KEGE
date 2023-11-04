//
//  Force.cpp
//  dev0
//
//  Created by Kenneth A Esdaile on 5/30/18.
//  Copyright © 2018 Kenneth Anthony Esdaile. All rights reserved.
//
#include "Force.hpp"
#include "Particle.hpp"
namespace kege{namespace efx{
    
    void Gravity::ApplyForce(const cg::point3& p, efx::Particle* particle)
    {
        particle->netforce += force;
    }
    Gravity::Gravity()
    :   force(0.0, -9.802 / 1000.0, 0.0)
    {
    }
    
    void Damping::ApplyForce(const cg::point3& p, efx::Particle* particle)
    {
        // cross-sectional area
        float area_circle = 3.14159 * particle->radius * particle->radius;
        // note that the velocity of the particle is normalized
        cg::vec3 drag = area_circle * particle->velocity * _AIR_DENSITY;
        particle->netforce -= drag;
    }
    Damping::Damping()
    {
    }
    
    void Wind::ApplyForce(const cg::point3& p, efx::Particle* particle)
    {
        float area_circle = 3.14159 * particle->radius * particle->radius;
        float wind_speed = 0.5 * _AIR_DENSITY * speed * speed * area_circle * _AIRDRAG_COEFFICIENT;
        particle->netforce += ((direction * wind_speed) / particle->mass);
    }
    Wind::Wind(const cg::vec3& dir, float speed)
    :   direction(dir)
    ,   speed(speed)
    {
    }
    Wind::Wind()
    :   direction(0.0)
    ,   speed(1.5)
    {
    }
    
    void AttractionForce::ApplyForce(const cg::point3& p, efx::Particle* particle)
    {
        cg::vec3 v = p-particle->position;
        float s = cg::magn(v);
        if (s >= 0.0005) v =  v / isnan(s);
        s = (s / radius);
        if (s > 1 ) return;
        particle->velocity += (v + cg::cross(v,cg::vec3::up)) * s * 0.125;
    }
    AttractionForce::AttractionForce()
    :   center(0.0)
    ,   radius(300)
    ,   mass(1.0)
    {
    }
}}


