#ifndef PARTICLE_H
#define PARTICLE_H

#define PARTICLE_MAX (1024 * 2048)

struct Particle {
    f32 life;
    v2 pos;
    v2 vel;
    v4 col;
};

struct ParticleSystem {
    i32 count;
    Particle particle[PARTICLE_MAX];
};

void SpawnParticle(ParticleSystem* ps, const Particle& particle);
static void RenderParticles(ParticleSystem* ps);

#endif //PARTICLE_H
