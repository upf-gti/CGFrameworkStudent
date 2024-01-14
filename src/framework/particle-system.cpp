#include "particle-system.h"

ParticleSystem::ParticleSystem()
{
    // Initialize all particles as inactive
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        particles[i].inactive = true;
    }
}

void ParticleSystem::Init()
{
    // Initialize all particles as inactive
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        particles[i].inactive = true;
        particles[i].position = Vector2(rand(), rand());
        particles[i].velocity = Vector2(rand(), rand());
        particles[i].color = Color(rand(), rand(), rand());
    }
}

void ParticleSystem::Render(Image *framebuffer)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        if (!particles[i].inactive)
        {
            framebuffer->SetPixelSafe(particles[i].position.x, particles[i].position.y, particles[i].color);
        }
    }
}

void ParticleSystem::Update(float dt)
{
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        if (!particles[i].inactive)
        {
            particles[i].position = particles[i].position + particles[i].velocity * dt;
            particles[i].ttl -= dt;
            if (particles[i].ttl <= 0)
            {
                particles[i].inactive = true;
            }
        }
    }
}