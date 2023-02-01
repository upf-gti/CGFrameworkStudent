#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"
#include "entity.h"

//if you de-comment the lines: 32,38,45 you'll se the animation.
Application::Application(const char* caption, int width, int height)
{
    this->window = createWindow(caption, width, height);

    int w,h;
    SDL_GetWindowSize(window,&w,&h);

    this->mouse_state = 0;
    this->time = 0.f;
    this->window_width = w;
    this->window_height = h;
    this->keystate = SDL_GetKeyboardState(nullptr);

    this->framebuffer.Resize(w, h);
}

Application::~Application()
{
    SDL_DestroyWindow(window);
}

void Application::Init(void)
{
    std::cout << "Initiating app..." << std::endl;

    Mesh* mesh = new Mesh();
    mesh->LoadOBJ("../res/meshes/lee.obj");

    Entity* e = new Entity(mesh);
    Camera c = Camera();
    c.SetPerspective(45, 1, .01, 100);
    e->Render(&framebuffer, &c, Color::WHITE);

}

// Render one frame
void Application::Render(void)
{
    framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{
    
}

//keyboard press event
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
    unsigned char r = rand() % 256;
    unsigned char g = rand() % 256;
    unsigned char b = rand() % 256;
    
    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch(event.keysym.sym) {
        case SDLK_1:
            framebuffer.DrawLineDDA(100, 100, 200, 300, Color::YELLOW);
            break;
        case SDLK_2:
            framebuffer.DrawLineBresenham(1000, 300, 1000 + 200 * cos(time * 3), 300 + 200 * sin(time * 3), Color(r, g, b));
            break;
        case SDLK_3:
            framebuffer.DrawCircle(500, 500, 100, Color::WHITE, true);
            break;
        case SDLK_4:
            framebuffer.Fill(Color::BLACK);
            break;
        case SDLK_ESCAPE:
            exit(0);
            break; // ESC key, kill the app
    }
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {

    }
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
    if (event.button == SDL_BUTTON_LEFT) {

    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    
}

void Application::OnFileChanged(const char* filename)
{
    Shader::ReloadSingleShader(filename);
}
