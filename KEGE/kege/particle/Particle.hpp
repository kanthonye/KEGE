/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 *  ParticleEffectsCreator
 *  Particle.h
 *  Created by Kenneth Anthony Esdaile on 8/24/15.
 *   Copyright (c) 2015 kae. All rights reserved.
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef __Particle_H__
#define __Particle_H__
#include "dlist.hpp"
#include "cgm.hpp"
namespace kege{namespace efx{
    
    struct Particle
    {
        cg::point3 position;  // Position
        cg::vec3 velocity;  // Velocity
        cg::vec3 impact_force;
        cg::vec3 netforce;  // Total force acting on the particle
        cg::vec4 color;
        
        float    restitution; // bouncieness
        float    radius;    // Particle radius used for collision detection
        float    mass;      // Total mass
        float    life;
        bool     collide;
    };
}}
#endif /* defined(__ParticleEffectsCreator__Particle__) */
