#include "particle-system.h"
#include <cstdlib>


ParticleSystem::ParticleSystem()
{
    //Init();
}

void ParticleSystem::Init()
{
    std::cout << "Initializing particle system" << std::endl;
    
    // Initialize all particles as inactive
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        particles[i].inactive = false;
        particles[i].position = Vector2(rand()%1280, rand()%720);
        particles[i].velocity = Vector2(rand()%10, rand()%10);
        particles[i].color = Color(255, 255, 255);
        particles[i].ttl = 100;
    }

    std::cout << "Particle system initialized" << std::endl;
}

void ParticleSystem::Render(Image *framebuffer)
{   
    if (particles->inactive == false)
    {
        std::cout << "Rendering particle system" << std::endl;

         for (int i = 0; i < 100; i++)
        {
            framebuffer->SetPixelSafe(particles[i].position.x, particles[i].position.y, particles[i].color);
        }
    }
}

void ParticleSystem::Update(float dt)
{
    std::cout << "Updating particle system" << std::endl;

    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        if (particles[i].inactive == false)
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
