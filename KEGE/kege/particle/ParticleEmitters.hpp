//
//  KEGE (Kenneth Esdaile Game Engine)
//  ParticleEmitters.hpp
//
//  Created by Kenneth Esdaile on 11/8/15.
//  Copyright © 2015 kanthonye. All rights reserved.
//

#ifndef ParticleEmitters_hpp
#define ParticleEmitters_hpp
#include "random.hpp"
#include "ParticleEmitter.hpp"
namespace kege{namespace efx{
    
    class CubicEffexParticleFactory : public ParticleEmitter
    {
    public:
        CubicEffexParticleFactory(float width, float height, float depth);
        cg::point3 GetSpawnPoint(scene::Gameobject& owner);
        
        void SetHeight(float len) {height = len;}
        void SetWidth(float len) {width = len;}
        void SetDepth(float len) {depth = len;}
        float GetHeight() {return height;}
        float GetWidth() {return width;}
        float GetDepth() {return depth;}
    private:
        float width, height, depth;
    };
}}

namespace kege{namespace efx{
    
    class PlaneAreaParticleFactory : public ParticleEmitter
    {
    public:
        PlaneAreaParticleFactory(float width, float height);
        cg::point3 GetSpawnPoint(scene::Gameobject& owner);
        
        void SetHeight(float len) {height = len;}
        void SetWidth(float len) {width = len;}
        float GetHeight() {return height;}
        float GetWidth() {return width;}
    private:
        float width, height;
    };
}}

namespace kege{namespace efx{
    
    class LinearParticleFactory : public ParticleEmitter
    {
    public:
        LinearParticleFactory(const cg::vec3& ray, float length);
        cg::point3 GetSpawnPoint(scene::Gameobject& owner);
        
        void SetRay(const cg::vec3& r) {ray = r;}
        cg::vec3& GetRay() {return ray;}
        void SetLength(const float l) {length = l;}
        float GetLength() {return length;}
    private:
        cg::vec3 ray;
        float length;
    };
}}

namespace kege{namespace efx{
    
    class PyrimidParticleFactory : public ParticleEmitter
    {
    public:
        PyrimidParticleFactory(float width, float height, float depth);
        cg::point3 GetSpawnPoint(scene::Gameobject& owner);
        
        void SetHeight(float len) {height = len;}
        void SetWidth(float len) {width = len;}
        void SetDepth(float len) {depth = len;}
        float GetHeight() {return height;}
        float GetWidth() {return width;}
        float GetDepth() {return depth;}
    private:
        float width, height, depth;
    };
}}

namespace kege{namespace efx{
    
    class TriangularParticleFactory : public ParticleEmitter
    {
    public:
        TriangularParticleFactory(float height, float base);
        cg::point3 GetSpawnPoint(scene::Gameobject& owner);
        
        void SetHeight(float len) {height = len;}
        void SetBase(float h) {base = h;}
        float GetHeight() {return height;}
        float GetBase() {return base;}
    private:
        float height, base;
    };
}}

namespace kege{namespace efx{
    
    class CircumEffexParticleFactory : public ParticleEmitter
    {
    public:
        CircumEffexParticleFactory(float radius);
        cg::point3 GetSpawnPoint(scene::Gameobject& owner);
        
        void SetRadius(float len) {radius = len;}
        float GetRadius() {return radius;}
    private:
        float radius;
    };
}}

namespace kege{namespace efx{
    
    class CircAreaEffexParticleFactory : public ParticleEmitter
    {
    public:
        CircAreaEffexParticleFactory(float min_radius, float max_radius);
        cg::point3 GetSpawnPoint(scene::Gameobject& owner);
        
        void SetMinRadius(float len) {min_radius = len;}
        void SetMaxRadius(float len) {max_radius = len;}
        float GetMinRadius() {return min_radius;}
        float GetMaxRadius() {return max_radius;}
    private:
        float min_radius, max_radius;
    };
}}

namespace kege{namespace efx{
    
    class SphereAreaEffexParticleFactory : public ParticleEmitter
    {
    public:
        SphereAreaEffexParticleFactory(float min_radius, float max_radius);
        cg::point3 GetSpawnPoint(scene::Gameobject& owner);
        
        void SetMinRadius(float len) {min_radius = len;}
        void SetMaxRadius(float len) {max_radius = len;}
        float GetMinRadius() {return min_radius;}
        float GetMaxRadius() {return max_radius;}
    private:
        float min_radius, max_radius;
    };
}}

namespace kege{namespace efx{
    
    class SphereEffexParticleFactory : public ParticleEmitter
    {
    public:
        SphereEffexParticleFactory(float radius);
        cg::point3 GetSpawnPoint(scene::Gameobject& owner);
        
        void SetRadius(float len) {radius = len;}
        float GetRadius() {return radius;}
    private:
        float radius;
    };
}}

namespace kege{namespace efx{
    
    class CylindricVolumeFactory : public ParticleEmitter
    {
    public:
        CylindricVolumeFactory(float height, float min_radius, float max_radius);
        cg::point3 GetSpawnPoint(scene::Gameobject& owner);
        
        void SetMinRadius(float len) {min_radius = len;}
        void SetMaxRadius(float len) {max_radius = len;}
        float GetMinRadius() {return min_radius;}
        float GetMaxRadius() {return max_radius;}
        void SetHeight(float len) {height = len;}
        float GetHeight() {return height;}
    private:
        float height, min_radius, max_radius;
    };
}}


namespace kege{namespace efx{
    
    class CylindricSurfaceFactory : public ParticleEmitter
    {
    public:
        CylindricSurfaceFactory(float height, float radius);
        cg::point3 GetSpawnPoint(scene::Gameobject& owner);
        
        void SetRadius(float len) {radius = len;}
        float GetRadius() {return radius;}
        void SetHeight(float len) {height = len;}
        float GetHeight() {return height;}
    private:
        float height, radius;
    };
}}


namespace kege{namespace efx{
    
    class ConeVolumeFactory : public ParticleEmitter
    {
    public:
        ConeVolumeFactory(float height, float min_radius, float max_radius);
        cg::point3 GetSpawnPoint(scene::Gameobject& owner);
        
        void SetXRadius(float len) {x_radius = len;}
        void SetZRadius(float len) {z_radius = len;}
        float GetXRadius() {return x_radius;}
        float GetZRadius() {return z_radius;}
        void SetHeight(float len) {height = len;}
        float GetHeight() {return height;}
    private:
        float height, x_radius, z_radius;
    };
}}



#endif /* ParticleEmitters_hpp */
