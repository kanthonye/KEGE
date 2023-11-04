//
//  ParticleEmission.cpp
//  Node Base Renderer
//
//  Created by Kenneth Esdaile on 11/8/15.
//  Copyright © 2015 kanthonye. All rights reserved.
//
#include "Gameobject.hpp"
#include "ParticleEmitters.hpp"
namespace kege{namespace efx{
    
    cg::point3 CubicEffexParticleFactory::GetSpawnPoint(scene::Gameobject& owner)
    {
        cg::point3 center = owner.GetTransform().position;
        return cg::point3
        ( (cg::randf(center.x-width, center.x+width)  * 0.5f),
         (cg::randf(center.y-height, center.y+height) * 0.5f),
         (cg::randf(center.z-depth , center.z+depth)  * 0.5f) );
    };
    CubicEffexParticleFactory::CubicEffexParticleFactory(float w, float h, float d)
    :   width(w)
    ,   height(h)
    ,   depth(d)
    {
//        velocity.x = cg::rand1f(0,0);
//        velocity.y = cg::rand1f(0,0);
//        velocity.z = cg::rand1f(0,0);
//        size = cg::rand1f(1,1.5);
    }
}}

namespace kege{namespace efx{
    
    PlaneAreaParticleFactory::PlaneAreaParticleFactory(float w, float h)
    :   width(w)
    ,   height(h)
    {
    }
    cg::point3 PlaneAreaParticleFactory::GetSpawnPoint(scene::Gameobject& owner)
    {
        return cg::point3(cg::randf(-width, width)* 0.5f, 0.0f, cg::randf(-height, height)*0.5f);
    }
}}

namespace kege{namespace efx{
    
    LinearParticleFactory::LinearParticleFactory(const cg::vec3& r, float l)
    :   ray(r)
    ,   length(l)
    {
    }
    cg::point3 LinearParticleFactory::GetSpawnPoint(scene::Gameobject& owner)
    {
        return cg::point3::zero + ray * cg::randf(0,length);
    }
}}

namespace kege{namespace efx{
    
    PyrimidParticleFactory::PyrimidParticleFactory(float w, float h, float d)
    :   width(w)
    ,   height(h)
    ,   depth(d)
    {
    }
    cg::point3 PyrimidParticleFactory::GetSpawnPoint(scene::Gameobject& owner)
    {
        float t = cg::randf(0, 1.0f);
        float h = cg::math::lerp(-height, height, t);
        
        float w = width*t;
        w = cg::randf(-w, w);
        
        float d = depth*t;
        d = cg::randf(-d, d);
        
        return cg::point3(w, h, d);
    }
}}

namespace kege{namespace efx{
    
    TriangularParticleFactory::TriangularParticleFactory(float h, float b)
    :   height(h)
    ,   base(b)
    {
    }
    cg::point3 TriangularParticleFactory::GetSpawnPoint(scene::Gameobject& owner)
    {
        float t = cg::randf(0, 1.0f);
        float h = cg::math::lerp(-height, height, t);
        
        float b = base*t;
        b = cg::randf(-b, b);
        
        return cg::point3(b, 0.0, h);
    }
}}

namespace kege{namespace efx{
    
    CircumEffexParticleFactory::CircumEffexParticleFactory(float r)
    :   radius(r)
    {
    }
    cg::point3 CircumEffexParticleFactory::GetSpawnPoint(scene::Gameobject& owner)
    {
        float radian = cg::randf(0, 6.28f);
        return cg::point3(cosf(radian)*radius, 0, sinf(radian)*radius);
    }
}}

namespace kege{namespace efx{
    
    CircAreaEffexParticleFactory::CircAreaEffexParticleFactory(float minr, float maxr)
    :   min_radius(minr)
    ,   max_radius(maxr)
    {
    }
    cg::point3 CircAreaEffexParticleFactory::GetSpawnPoint(scene::Gameobject& owner)
    {
        float radian = cg::randf(0, 6.28f);
        float radius = cg::randf(min_radius, max_radius);
        return owner.GetTransform().position + cg::point3(sinf(radian)*radius, 0.0, cosf(radian)*radius);
    }
}}

namespace kege{namespace efx{
    
    SphereAreaEffexParticleFactory::SphereAreaEffexParticleFactory(float minr, float maxr)
    :   min_radius(minr)
    ,   max_radius(maxr)
    {
    }
    
    cg::point3 SphereAreaEffexParticleFactory::GetSpawnPoint(scene::Gameobject& owner)
    {
        float radianA = cg::randf(0, 6.28f);
        float radianB = cg::randf(0, 6.28f);
        float radius = cg::randf(min_radius, max_radius);
        float y = sinf(radianB)*radius;
        float r = cosf(radianB)*radius;
        float z = sinf(radianA)*r;
        float x = cosf(radianA)*r;
        return cg::point3(x, y, z) + owner.GetTransform().position;
    }
}}

namespace kege{namespace efx{
    
    SphereEffexParticleFactory::SphereEffexParticleFactory(float r)
    :   radius(r)
    {
    }
    cg::point3 SphereEffexParticleFactory::GetSpawnPoint(scene::Gameobject& owner)
    {
        float radianA = cg::randf(0, 6.28f);
        float radianB = cg::randf(0, 6.28f);
        float y = sinf(radianB)*radius;
        float r = cosf(radianB)*radius;
        float z = sinf(radianA)*r;
        float x = cosf(radianA)*r;
        
        return cg::point3(x, y, z) + owner.GetTransform().position;
    }
}}


namespace kege{namespace efx{
    
    CylindricVolumeFactory::CylindricVolumeFactory(float h, float minr, float maxr)
    :   height(h)
    ,   min_radius(minr)
    ,   max_radius(maxr)
    {
    }
    cg::point3 CylindricVolumeFactory::GetSpawnPoint(scene::Gameobject& owner)
    {
        float h = cg::randf(0, height);
        float radian = cg::randf(0, 6.28f);
        float radius = cg::randf(min_radius, max_radius);
        float z = sinf(radian)*radius;
        float x = cosf(radian)*radius;
        return cg::point3(x, h, z);
    }
}}

namespace kege{namespace efx{
    
    CylindricSurfaceFactory::CylindricSurfaceFactory(float h, float r)
    :   height(h)
    ,   radius(r)
    {
    }
    cg::point3 CylindricSurfaceFactory::GetSpawnPoint(scene::Gameobject& owner)
    {
        float radian = cg::randf(0, 6.28f);
        float z = sinf(radian)*radius;
        float x = cosf(radian)*radius;
        float y = cg::randf(0, height);
        return cg::point3(x, y, z);
    }
}}

namespace kege{namespace efx{
    
    ConeVolumeFactory::ConeVolumeFactory(float h, float minr, float maxr)
    :   height(h)
    ,   x_radius(minr)
    ,   z_radius(maxr)
    {
    }
    cg::point3 ConeVolumeFactory::GetSpawnPoint(scene::Gameobject& owner)
    {
        float t = cg::randf(0, 1.0f);
        float h = cg::math::lerp(-height, height, t);
        
        float radian = cg::randf(0, 6.28f);
        float z = sinf(radian)*t*z_radius;
        float x = cosf(radian)*t*x_radius;
        return cg::point3(x, h, z);
    }
}}



