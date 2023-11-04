//
//  Force.hpp
//  dev0
//
//  Created by Kenneth A Esdaile on 5/30/18.
//  Copyright © 2018 Kenneth Anthony Esdaile. All rights reserved.
//

#ifndef Force_hpp
#define Force_hpp
#include "vec3.hpp"
#define _AIR_DENSITY 0.0125
#define _AIRDRAG_COEFFICIENT 0.0625
namespace kege{namespace efx{
    class Particle;
    
    class Force
    {
    public:
        virtual void ApplyForce(const cg::point3& p, efx::Particle* particle) = 0;
        virtual ~ Force(){}
    };
    
    class Gravity : public Force
    {
    public:
        void ApplyForce(const cg::point3& p, efx::Particle* particle);
        Gravity();
    protected:
        cg::vec3 force;
    };
    
    class Damping : public Force
    {
    public:
        void ApplyForce(const cg::point3& p, efx::Particle* particle);
        Damping();
    };
    
    class Wind : public Force
    {
    public:
        void ApplyForce(const cg::point3& p, efx::Particle* particle);
        Wind(const cg::vec3& dir, float speed);
        Wind();
    protected:
        cg::vec3 direction;
        float speed;
    };
    
    class AttractionForce : public Force
    {
    public:
        void ApplyForce(const cg::point3& p, efx::Particle* particle);
        AttractionForce();
    protected:
        cg::point3 center;
        float radius;
        float mass;
    };
}}
#endif /* Force_hpp */
