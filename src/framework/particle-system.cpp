#include "particle-system.h"
#include <cstdlib>

ParticleSystem::ParticleSystem()
{
    // Init();
}

void ParticleSystem::Init()
{

    // Initialize all particles as inactive
    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        particles[i].inactive = false;
        particles[i].position = Vector2(rand() % 1280, rand() % 720);
        particles[i].velocity = Vector2(rand() % (30 * 2 + 1) - 30, -rand() % 40);
        particles[i].color = Color(rand() % 255, rand() % 255, rand() % 255);
        particles[i].ttl = 100;
    }
}

void ParticleSystem::Render(Image *framebuffer)
{

    if (particles->inactive == false)
    {
        for (int i = 0; i < MAX_PARTICLES; i++)
        {

            // Limpiar la posición anterior dibujando un círculo del color de fondo
            framebuffer->DrawCircle(particles[i].lastPosition.x, particles[i].lastPosition.y, 2, Color::BLACK, 1, true, Color::BLACK);

            // Dibujar la partícula en la nueva posición como un círculo semi-redondo
            framebuffer->DrawCircle(particles[i].position.x, particles[i].position.y, 2, particles[i].color, 1, true, particles[i].color);
        }
    }
}

void ParticleSystem::Update(float dt)
{
    // std::cout << "Updating particle system" << std::endl;

    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        if (particles[i].inactive == false)
        {

            particles[i].lastPosition = particles[i].position; // Calculamos la posicion anterior de la particula

            particles[i].position = particles[i].position + particles[i].velocity * dt; // Calculamos la nueva posicion de la particula en funcion de la ecuacion de movimiento
            particles[i].ttl -= dt;                                                     // Calculamos el tiempo que le queda a la particula
            // particles[i].color = Color(rand() % 56 + 120, rand() % 56 + 120, rand() % 56 + 120); // Creamos un color aleatorio para la particula en cada uno de los frames
            if (particles[i].ttl <= 0) // Cuando la particula tenga que desaparecer
            {
                particles[i].inactive = true;
            }
        }
    }
}
