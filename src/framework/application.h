/*
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "button.h"
#include "particle-system.h"
#include "entity.h"
#include "camera.h"

// Necesitamos un estado para saber si estamos dibujando o no y el que
enum DrawingState
{
	NOT_DRAWING,
	DRAWING_FREE,
	DRAWING_LINE,
	DRAWING_RECTANGLE,
	DRAWING_CIRCLE,
	DRAWING_TRIANGLE,
	DRAWING_ANIMATION,
};

enum CameraState
{
	DRAW_SINGLE,
	DRAW_MULTIPLE
};

enum PropertyState
{
	CAMERA_NEAR,
	CAMERA_FAR,
	CAMERA_NONE
};

// Constantes para el tamaño del borde
#define MAX_BORDER_WIDTH 100
#define MIN_BORDER_WIDTH 1

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

	// Declarar y crear un sistema de particulas
	ParticleSystem particleSystem;

	// Declarar y crear una entidad
	Entity entity1, entity2, entity3;

	// Declarar y crear una cámara
	Camera *camera;

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
		tempbuffer.Resize(width, height);  // Lab1: Resize tempbuffer adaptandolo a la nueva resolucion de la pantalla
	}

	Vector2 GetWindowSize()
	{
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		return Vector2(float(w), float(h));
	}

	// LAB 1: Ajustes de borde
	int borderWidth = 2;
	std::vector<Button> toolbarButtons; // Botones en el Toolbar

	// Estado del dibujo
	DrawingState currentState = DRAWING_FREE;
	Color drawingColor = Color::WHITE;
	bool isFilled = false;

	// Vector para almacenar los puntos de los dibujos (temporal)
	std::vector<Vector2> puntos;

	// Lista de entidades para almacenar los dibujos
	std::vector<Entity> entities;

	void DrawCirclesDDA(Vector2 start, Vector2 end, int radius, const Color &color);
	// Última posición del ratón para pintar y que no se vea el trazo
	Vector2 lastMousePosition = {-1, -1};

	bool clickedOnToolbarButton = false;

	// Buffer temporal para almacenar la imagen antes de dibujarla en el framebuffer, si se tiene que restaurar o cargar mientras se está previsualizando alguna cosa.
	Image tempbuffer;

	// Estado de la camara
	CameraState cameraState;
	PropertyState propertyState = CAMERA_NONE;

	// Configuración de la cámara
	float fov = 45.0f;							 // Campo de visión
	float aspect = window_width / window_height; // Relación de aspecto
	float near_plane = 0.01;					 // Plano cercano
	float far_plane = 100.0;					 // Plano lejano

	// Variables para rastrear la posición previa del ratón
	int last_mouse_x, last_mouse_y;
};
