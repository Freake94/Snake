
void AddParticle(ParticleSystem* ps, const Particle& particle) {
    if(ps->count >= PARTICLE_MAX) return;
    ps->particle[ps->count++] = particle;
}

static void UpdateParticles(ParticleSystem* ps, f32 dt) {
    for(int i = 0; i < ps->count; i++) {
        Particle* p = &ps->particle[i];
        p->pos += p->vel * dt;
        p->vel -= p->vel * dt;
        p->life -= dt;
        
        if(p->life <= 0.0f) {
            ps->particle[i--] = ps->particle[--ps->count];
        }
    }
}

static void RenderParticles(ParticleSystem* ps) {
    for(int i = 0; i < ps->count; i++) {
        Particle* p = &ps->particle[i];
        RenderParticle({ p->pos.x - 0.3f, p->pos.y - 0.3f },
                       { p->pos.x + 0.3f, p->pos.y + 0.3f },
                       { p->col.r, p->col.g, p->col.b, p->col.a * (p->life / 0.3f) });
    }
}