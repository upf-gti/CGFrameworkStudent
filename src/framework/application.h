/*
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "button.h"

// Necesitamos un estado para saber si estamos dibujando o no y el que
enum DrawingState
{
	NOT_DRAWING,
	DRAWING_FREE,
	DRAWING_LINE,
	DRAWING_RECTANGLE,
	DRAWING_CIRCLE,
	DRAWING_TRIANGLE
};

class Application
{
public:
	// Window

	SDL_Window *window = nullptr;
	int window_width;
	int window_height;

	float time;

	// Input
	const Uint8 *keystate;
	int mouse_state;		// Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta;	// Mouse movement in the last frame

	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnWheel(SDL_MouseWheelEvent event);
	void OnFileChanged(const char *filename);

	// CPU Global framebuffer
	Image framebuffer;

	// Constructor and main methods
	Application(const char *caption, int width, int height);
	~Application();

	void Init(void);
	void Render(void);
	void Update(float dt);

	// Other methods to control the app
	void SetWindowSize(int width, int height)
	{
		glViewport(0, 0, width, height);
		this->window_width = width;
		this->window_height = height;

		framebuffer.Resize(width, height); // Lab1: Resize framebuffer adaptandolo a la nueva resolucion de la pantalla
	}

	Vector2 GetWindowSize()
	{
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		return Vector2(float(w), float(h));
	}

	// LAB 1: Ajustes de borde
	int borderWidth = 1;
	std::vector<Button> toolbarButtons; // Botones en el Toolbar

	// Estado del dibujo
	DrawingState currentState = DRAWING_FREE;
	Color drawingColor = Color::WHITE;
	bool isFilled = false;

	// Vector para almacenar los puntos de los dibujos (temporal)
	std::vector<Vector2> puntos;

	// Estado para saber si el boton está presionado o no
	bool isLeftButtonPressed = false;

	void DrawCirclesDDA(Vector2 start, Vector2 end, int radius, const Color &color);
	// Última posición del ratón para pintar y que no se vea el trazo
	Vector2 lastMousePosition = {-1, -1};
};
