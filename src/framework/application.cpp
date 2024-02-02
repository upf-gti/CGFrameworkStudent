#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h"

Application::Application(const char *caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	this->mouse_state = 0;
	this->time = 0.f;
	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(nullptr);

	this->tempbuffer.Resize(w, h);
	this->framebuffer.Resize(w, h);
}

Application::~Application()
{
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;

	framebuffer.Fill(Color::BLACK);

	/* Creamos un loop para cargar todas las imagenes una al lado del otra con un loop
	std::vector<const char *> imagePaths = {
		"../res/images/clear.png",
		"../res/images/load.png",
		"../res/images/save.png",
		"../res/images/eraser.png",
		"../res/images/paint.png",
		"../res/images/line.png",
		"../res/images/rectangle.png",
		"../res/images/circle.png",
		"../res/images/triangle.png",
		"../res/images/black.png",
		"../res/images/white.png",
		"../res/images/pink.png",
		"../res/images/yellow.png",
		"../res/images/red.png",
		"../res/images/blue.png",
		"../res/images/cyan.png",
		"../res/images/green.png"};

	std::vector<ButtonType> buttonTypes = {BTN_CLEAR, BTN_LOAD, BTN_SAVE, BTN_ERASER, BTN_PAINT, BTN_LINE, BTN_RECTANGLE, BTN_CIRCLE, BTN_TRIANGLE, BTN_BLACK, BTN_WHITE, BTN_PINK, BTN_YELLOW, BTN_RED, BTN_BLUE, BTN_CYAN, BTN_GREEN};

	int toolbarIndexX = 10;

	for (size_t i = 0; i < imagePaths.size(); ++i)
	{
		// Crea cada botón con su imagen y tipo correspondiente
		toolbarButtons.emplace_back(imagePaths[i], Vector2(toolbarIndexX, 10), buttonTypes[i]);
		toolbarIndexX += toolbarButtons.back().GetImage().width + 10; // Cogemos image y le sumamos 10 para que haya un espacio entre cada botón razonable
	}

	bool inactive = true;
	*/

	// Cargar las 3 mallas
	Mesh *mesh_lee = new Mesh();
	mesh_lee->LoadOBJ("meshes/lee.obj");

	Mesh *mesh_anna = new Mesh();
	mesh_anna->LoadOBJ("meshes/anna.obj");

	Mesh *mesh_cleo = new Mesh();
	mesh_cleo->LoadOBJ("meshes/cleo.obj");

	// Asignar la malla a las entidades
	entity1.mesh = *mesh_lee;
	entity2.mesh = *mesh_anna;
	entity3.mesh = *mesh_cleo;

	// Establecer las matrices de modelo para posicionar las entidades
	entity1.modelMatrix.SetTranslation(0, 0.2, -0.4);	  // Posiciona entity1
	entity2.modelMatrix.SetTranslation(-0.5, -0.1, -0.5); // Posiciona entity2 en el origen
	entity3.modelMatrix.SetTranslation(0.3, -0.4, -0.5);  // Posiciona entity3

	camera = new Camera();

	// Configurar la vista de la cámara y la perspectiva
	// camera->Move(Vector3(0, 0, 25)); // Mover la cámara hacia atrás
	camera->LookAt(Vector3(0, 0.2, 0.75), Vector3(0, 0.2, 0), Vector3::UP);
	camera->SetPerspective(fov, aspect, near_plane, far_plane);

	// Añadir las entidades a la lista
	entities.push_back(entity1);
	entities.push_back(entity2);
	entities.push_back(entity3);
}

void Application::Render(void)
{
	if (cameraState == DRAW_SINGLE)
	{
		entity1.Render(&framebuffer, camera, Color::WHITE);
	}
	else if (cameraState == DRAW_MULTIPLE)
	{
		entity1.Render(&framebuffer, camera, Color::WHITE);
		entity2.Render(&framebuffer, camera, Color::RED);
		entity3.Render(&framebuffer, camera, Color::BLUE);
	}

	framebuffer.Render(); // Renderizamos el framebuffer
}

void Application::Update(float seconds_elapsed)
{
	/* Lab1
	if (currentState == DRAWING_ANIMATION)
	{
		particleSystem.Update(seconds_elapsed);
	}
	*/

	if (cameraState == DRAW_MULTIPLE)
	{
		entity1.modelMatrix.RotateLocal(seconds_elapsed * 10 * DEG2RAD, Vector3(0, 1, 0));

		float current_scale = 1.0f; // Factor de escala inicial
		float scale_speed = 0.01f;	// Velocidad de cambio de la escala
		float min_scale = 0.5f;		// Escala mínima
		float max_scale = 3.0f;		// Escala máxima

		// Calculamos el cambio de escala basado en el tiempo
		float scale_change = sin(time) * scale_speed;
		current_scale += scale_change;

		// Limitamos la escala entre los valores mínimo y máximo
		current_scale = std::max(min_scale, std::min(max_scale, current_scale));

		// Aplicar la escala a la entidad
		entity2.modelMatrix.Scale(current_scale, current_scale, current_scale);

		// Reducimos la velocidad
		scale_speed *= 0.99f;

		entity3.modelMatrix.Translate(0.01 * sin(time), 0, 0);
	}

	// Actualizar y dibujar las entidades dependiendo del estado actual
	switch (cameraState)
	{
	case DRAW_SINGLE:
		// Dibujar una sola entidad
		framebuffer.Fill(Color::BLACK);
		entities[0].Update(seconds_elapsed);
		break;
	case DRAW_MULTIPLE:
		// Dibujar múltiples entidades
		for (Entity &entity : entities)
		{
			framebuffer.Fill(Color::BLACK);
			entity.Update(seconds_elapsed);
		}
		break;
	}
}

// keyboard press event
void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode

	/* Keystoke Console Debug */
	std::cout << "Key pressed: " << event.keysym.sym << std::endl;
	std::cout << borderWidth << std::endl;

	switch (event.keysym.sym)
	{
	case SDLK_ESCAPE:
		exit(0);
		break; // ESC key, kill the app
	case SDLK_PLUS:
		if (propertyState == CAMERA_NEAR)
		{
			camera->near_plane = std::min(camera->near_plane + 0.15f, camera->far_plane - 0.15f); // Incrementa de a poco el near_plane, asegurándose de que sea siempre menor que far_plane
		}
		else if (propertyState == CAMERA_FAR)
		{
			camera->far_plane += camera->far_plane = std::max(camera->near_plane + 0.2f, camera->far_plane); // Incrementa de a poco el far_plane
		}
		else if (camera->type == Camera::PERSPECTIVE)
		{
			camera->fov = std::min(179.0f, camera->fov + 3.0f); // Incrementa el FOV, asegurándose de que no sea mayor a 179
		}
		camera->UpdateProjectionMatrix();
		break;
	case SDLK_MINUS:
		if (propertyState == CAMERA_NEAR)
		{
			camera->near_plane = std::max(0.15f, camera->near_plane - 0.15f); // Decrementa de a poco el near_plane, no permite que sea menor a 0.01
		}
		else if (propertyState == CAMERA_FAR)
		{
			camera->far_plane = std::max(camera->near_plane + 0.2f, camera->far_plane - 0.2f); // Decrementa de a poco el far_plane, asegurándose de que sea siempre mayor que near_plane
		}
		else if (camera->type == Camera::PERSPECTIVE)
		{
			camera->fov = std::max(1.0f, camera->fov - 3.0f); // Decrementa el FOV en incrementos más pequeños
		}
		camera->UpdateProjectionMatrix();
		break;
	case SDLK_1:
		cameraState = DRAW_SINGLE; // Cambia a dibujar una sola entidad
		break;
	case SDLK_2:
		cameraState = DRAW_MULTIPLE; // Cambia a dibujar entidades animadas
		break;
	case SDLK_o:
	{
		camera->type = Camera::ORTHOGRAPHIC;

		float aspect_ratio = static_cast<float>(window_width) / static_cast<float>(window_height);
		float ortho_size = 1.0f;

		camera->SetOrthographic(-ortho_size * aspect_ratio, ortho_size * aspect_ratio, -ortho_size, ortho_size, camera->near_plane, camera->far_plane);
		camera->UpdateProjectionMatrix();
		break;
	}
	case SDLK_p:
		camera->type = Camera::PERSPECTIVE;
		propertyState = CAMERA_NONE;
		camera->SetPerspective(camera->fov, static_cast<float>(window_width) / static_cast<float>(window_height), camera->near_plane, camera->far_plane);
		camera->UpdateProjectionMatrix();
		break;
	case SDLK_n:
		propertyState = CAMERA_NEAR; // Ajustar el near plane
		break;
	case SDLK_f:
		propertyState = CAMERA_FAR; // Ajustar el far plane
		break;
	case SDLK_b:
		/*Lab1: Relleno
		isFilled = !isFilled; // Ponemos el valor contrario
		*/
		framebuffer.Fill(Color::BLACK);
		break;
	}
}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
	// std::cout << "Mouse button pressed: " << (int)event.button << SDL_BUTTON_LEFT << std::endl;
}

void Application::OnMouseButtonUp(SDL_MouseButtonEvent event)
{
	// std::cout << "Mouse button released: " << (int)event.button << std::endl;
}

void Application::DrawCirclesDDA(Vector2 p0, Vector2 p1, int radius, const Color &color)
{

	// Algrotimo similar al de la linea con DDA, pero usando circulos para rellenar el espacio entre los puntos, no pixeles, para un trazo mas suave

	int dx = p1.x - p0.x;
	int dy = p1.y - p0.y;
	int d = std::max(abs(dx), abs(dy));

	float xInc = dx / (float)d;
	float yInc = dy / (float)d;

	float x = p0.x;
	float y = p0.y;

	for (int i = 0; i <= d; i++)
	{
		framebuffer.DrawCircle((int)(x), (int)(y), radius, color, 0, true, color);
		x += xInc;
		y += yInc;
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	if (mouse_state == SDL_BUTTON_RIGHT || mouse_state == SDL_BUTTON_X1)
	{
		// Calcular el cambio de posición del ratón
		int delta_x = event.x - last_mouse_x;
		int delta_y = event.y - last_mouse_y;

		// Calcular el vector de desplazamiento en el espacio del mundo
		Vector3 right = camera->GetLocalVector(Vector3::RIGHT);
		Vector3 up = camera->GetLocalVector(Vector3::UP);
		Vector3 delta_position = right * (float)delta_x * 0.001f - up * (float)delta_y * 0.001f;

		// Mover el centro de la cámara basándose en el desplazamiento calculado
		camera->center = camera->center + delta_position;

		camera->UpdateViewMatrix();
	}
	else if (mouse_state == SDL_BUTTON_LEFT)
	{
		int delta_x = event.x - last_mouse_x;
		int delta_y = event.y - last_mouse_y;

		float angle_y = delta_x * 0.08;
		float angle_x = -delta_y * 0.05;

		// Calcular la nueva posición de 'eye' para la rotación horizontal
		Vector3 center_to_eye = camera->eye - camera->center;
		Vector3 horizontal_axis = Vector3::UP; // Eje de rotación horizontal (arriba global)

		// Rotar 'center_to_eye' alrededor del eje Y global
		Matrix44 rot_y;
		rot_y.SetIdentity();
		rot_y.Rotate(angle_y, horizontal_axis);
		center_to_eye = rot_y.RotateVector(center_to_eye);

		// Calcular eje de rotación vertical (perpendicular a 'center_to_eye' y 'UP')
		Vector3 vertical_axis = center_to_eye.Cross(Vector3::UP).Normalize();

		// Rotar 'center_to_eye' alrededor del nuevo eje vertical
		Matrix44 rot_x;
		rot_x.SetIdentity();
		rot_x.Rotate(angle_x, vertical_axis);
		center_to_eye = rot_x.RotateVector(center_to_eye);

		// Actualizar 'eye' con la nueva posición calculada
		camera->eye = camera->center + center_to_eye;

		camera->UpdateViewMatrix();
	}

	// Actualizar la posición anterior del ratón
	last_mouse_x = event.x;
	last_mouse_y = event.y;
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;

	// Añadimos limites para que no se pueda hacer infinitamente grande o pequeño, funcionalidad propia, para aumentar o disminuir el grosor del borde, en funcion de la rueda del ratón
	if (dy > 0)
	{
		camera->eye = camera->eye * 0, 95;
	}
	else if (dy < 0)
	{
		camera->eye = camera->eye * 1, 05;
	}
}

void Application::OnFileChanged(const char *filename)
{
	Shader::ReloadSingleShader(filename);
}
