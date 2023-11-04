#  Creating ParticleSystem

~~~
scene::Gameobject* particle_system = new scene::Gameobject;
particle_system->AddComponent(new efx::ParticleSystem);
particle_system->AddComponent(new efx::ParticleEmitter);
particle_system->AddComponent(new efx::EmissionTimer(10, 20));
~~~


