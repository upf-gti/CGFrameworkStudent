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

	// Creamos un loop para cargar todas las imagenes una al lado del otra con un loop
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
}

void Application::Render(void)
{
	// Establecemos el color de la linea
	Color color = Color::WHITE;

	/* Figuras Testing - NO DESCOMENTAR
	// Establecemos el punto de origen de la linea en el centro de la pantalla
	int x = this->window_width / 2;
	int y = this->window_height / 2;
	framebuffer.DrawRect(x, y, 220, 100, color, 2, true, Color::PURPLE);

	x = this->window_width / 4;
	y = this->window_height / 2;

	framebuffer.DrawLineDDA(x, y, x + 100 * cos(time), y + 100 * sin(time), color);

	x = this->window_width / 3;
	y = this->window_height / 6;

	framebuffer.DrawCircle(x, y, 100, color, 2, true, Color::RED);

	Vector2 p0(100, 100); // Primer punto
	Vector2 p1(300, 300); // Segundo punto
	Vector2 p2(100, 300); // Tercer punto

	framebuffer.DrawTriangle(p0, p1, p2, color, true, Color::GREEN);
	*/

	// Dibujamos el toolbar, en cada Render() para que se actualice
	framebuffer.DrawRect(0, 0, this->window_width, 50, Color::GRAY, 2, true, Color::GRAY);

	for (Button &button : toolbarButtons) // Creamos todos los botones
	{
		button.Render(framebuffer);
	}

	if (currentState == DRAWING_ANIMATION) // En caso de estar dibujando las particulas, las renderizamos
	{
		particleSystem.Render(&framebuffer);
	}

	framebuffer.Render(); // Renderizamos el framebuffer
}

void Application::Update(float seconds_elapsed)
{
	if (currentState == DRAWING_ANIMATION)
	{
		particleSystem.Update(seconds_elapsed);
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
		if (borderWidth < MAX_BORDER_WIDTH)
			borderWidth++; // Ponemos un limite por cuestiones de rendimiento
		break;
	case SDLK_MINUS:
		if (borderWidth > MIN_BORDER_WIDTH)
			borderWidth--; // Debemos tener en cuenta que el borde no puede ser negativo
		break;
	case SDLK_1:
		// Draw a line
		currentState = DRAWING_LINE;
		puntos.clear();
		break;
	case SDLK_2:
		// Draw rectangles
		currentState = DRAWING_RECTANGLE;
		puntos.clear();
		break;
	case SDLK_3:
		// Draw a circle
		currentState = DRAWING_CIRCLE;
		puntos.clear();
		break;
	case SDLK_4:
		// Draw triangles
		currentState = DRAWING_TRIANGLE;
		puntos.clear();
		break;
	case SDLK_5:
		// Paint
		currentState = DRAWING_FREE;
		puntos.clear();
		break;
	case SDLK_6:
		// Animation
		currentState = DRAWING_ANIMATION;
		particleSystem.Init();
		puntos.clear();
		break;
	case SDLK_f:
		// Fill Shapes
		isFilled = !isFilled; // Ponemos el valor contrario
		break;
	}
}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
	// std::cout << "Mouse button pressed: " << (int)event.button << SDL_BUTTON_LEFT << std::endl;
	clickedOnToolbarButton = false;

	if ((int)event.button == SDL_BUTTON_LEFT) // Miramos que sea el botón izquierdo
	{

		for (Button &button : toolbarButtons) // Para cada boton creado de la toolbar comporoamos si se ha hecho click en el
		{
			if (button.IsMouseInside(mouse_position))
			{
				clickedOnToolbarButton = true;

				// Usando el método getType() para obtener el tipo de botón
				ButtonType type = button.GetType();

				// Imprimir el tipo de botón
				std::cout << "Click en el boton: ";
				switch (type)
				{
				case BTN_CLEAR:
					std::cout << "Clear";
					framebuffer.Fill(Color::BLACK);
					break;
				case BTN_LOAD:
					std::cout << "Load";
					framebuffer.LoadPNG("../res/images/gos.png");
					break;
				case BTN_SAVE:
					std::cout << "Save";
					framebuffer.SaveTGA("../res/images/save.tga");
					break;
				case BTN_ERASER:
					std::cout << "Eraser";
					currentState = DRAWING_FREE;
					drawingColor = Color::BLACK;
					puntos.clear();
					break;
				case BTN_PAINT:
					std::cout << "Paint";
					currentState = DRAWING_FREE;
					puntos.clear();
					break;
				case BTN_LINE:
					std::cout << "Line";
					currentState = DRAWING_LINE;
					puntos.clear();
					break;
				case BTN_RECTANGLE:
					std::cout << "Rectangle";
					currentState = DRAWING_RECTANGLE;
					puntos.clear();
					break;
				case BTN_CIRCLE:
					std::cout << "Circle";
					currentState = DRAWING_CIRCLE;
					puntos.clear();
					break;
				case BTN_TRIANGLE:
					std::cout << "Triangle";
					currentState = DRAWING_TRIANGLE;
					puntos.clear();
					break;
				case BTN_BLACK:
					std::cout << "Black";
					drawingColor = Color::BLACK;
					break;
				case BTN_WHITE:
					std::cout << "White";
					drawingColor = Color::WHITE;
					break;
				case BTN_PINK:
					std::cout << "Pink";
					drawingColor = Color::PURPLE;
					break;
				case BTN_YELLOW:
					std::cout << "Yellow";
					drawingColor = Color::YELLOW;
					break;
				case BTN_RED:
					std::cout << "Red";
					drawingColor = Color::RED;
					break;
				case BTN_BLUE:
					std::cout << "Blue";
					drawingColor = Color::BLUE;
					break;
				case BTN_CYAN:
					std::cout << "Cyan";
					drawingColor = Color::CYAN;
					break;
				case BTN_GREEN:
					std::cout << "Green";
					drawingColor = Color::GREEN;
					break;
				default:
					std::cout << "Othero";
				}
				std::cout << std::endl; // Salto de linea
				break;					// Un break para que no se siga ejecutando el bucle, ya que hemos encontrado el botón (no se puede hacer click a dos a la vez)
			}
		}

		if (currentState != NOT_DRAWING && currentState != DRAWING_FREE && !clickedOnToolbarButton) // Si se está dibujando algo
		{
			if (currentState == DRAWING_TRIANGLE || puntos.size() == 0)
			{
				puntos.push_back(mouse_position);

				tempbuffer = framebuffer; // Guardamos el framebuffer en el temporal
				// Añadimos un punto al vector de puntos
				std::cout << "Punto añadido: " << mouse_position.x << ", " << mouse_position.y << std::endl;
				std::cout << "Puntos en el vector: " << puntos.size() << std::endl;
				std::cout << "Estado actual: " << currentState << std::endl;
				switch (currentState)
				{
				case DRAWING_TRIANGLE:
					if (puntos.size() == 3)
					{
						framebuffer.DrawTriangle(puntos[0], puntos[1], puntos[2], drawingColor, isFilled, Color::GREEN); // Hardcodeamos el color del interior, para que podamos diferenciarlo del borde
						puntos.clear();
					}
					break;
				}
			}
		}
	}
}

void Application::OnMouseButtonUp(SDL_MouseButtonEvent event)
{
	// std::cout << "Mouse button released: " << (int)event.button << std::endl;

	if ((int)event.button == SDL_BUTTON_LEFT) // Miramos que sea el botón izquierdo
	{
		if (currentState != NOT_DRAWING && currentState != DRAWING_FREE && currentState != DRAWING_TRIANGLE && !clickedOnToolbarButton)
		{
			puntos.push_back(mouse_position);

			switch (currentState)
			{
			case DRAWING_LINE:
				if (puntos.size() == 2)
				{
					puntos.clear();
				}
				break;
			case DRAWING_RECTANGLE:
				if (puntos.size() == 2)
				{
					puntos.clear();
				}
				break;
			case DRAWING_CIRCLE:
				if (puntos.size() == 2)
				{
					puntos.clear();
				}
				break;
			default:
				break;
			}
		}
	}
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
	if (mouse_state == SDL_BUTTON_LEFT) // Miramos si se está pintando y si se está pintando libremente
	{
		if (currentState == DRAWING_FREE)
		{
			if (lastMousePosition.x != -1 && lastMousePosition.y != -1) // Miramos si es la primera vez que se pinta
			{
				// Dibuja círculos interpolados entre la última posición y la actual
				DrawCirclesDDA(lastMousePosition, mouse_position, borderWidth / 2, drawingColor);
			}
		}
		else if (currentState != NOT_DRAWING && currentState != DRAWING_FREE && currentState != DRAWING_TRIANGLE && !clickedOnToolbarButton)
		{
			if (currentState == DRAWING_LINE && !puntos.empty())
			{
				// Restaura el estado del framebuffer desde el buffer temporal.
				framebuffer = tempbuffer;

				// Dibuja la nueva línea temporal.
				framebuffer.DrawLineDDA(puntos[0].x, puntos[0].y, mouse_position.x, mouse_position.y, drawingColor);
			}
			else if (currentState == DRAWING_RECTANGLE && !puntos.empty())
			{
				// Restaura el estado del framebuffer desde el buffer temporal.
				framebuffer = tempbuffer;

				// Dibuja la nueva línea temporal.
				framebuffer.DrawRect(puntos[0].x, puntos[0].y, mouse_position.x - puntos[0].x, mouse_position.y - puntos[0].y, drawingColor, borderWidth, isFilled, Color::GREEN); // Hardcodeamos el color del interior, para que podamos diferenciarlo del borde
			}
			else if (currentState == DRAWING_CIRCLE && !puntos.empty())
			{
				framebuffer = tempbuffer;
				framebuffer.DrawCircle(puntos[0].x, puntos[0].y, sqrt(pow(mouse_position.x - puntos[0].x, 2) + pow(mouse_position.y - puntos[0].y, 2)), drawingColor, borderWidth, isFilled, Color::GREEN); // Hardcodeamos el color del interior, para que podamos diferenciarlo del borde
			}
		}

		lastMousePosition = mouse_position; // Cambiamos la última posición por la actual
	}
	else // Si no se está pintando libremente, se resetea la última posición
	{
		lastMousePosition = {-1, -1};
	}
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;

	// Añadimos limites para que no se pueda hacer infinitamente grande o pequeño, funcionalidad propia, para aumentar o disminuir el grosor del borde, en funcion de la rueda del ratón
	if (dy > 0)
	{
		borderWidth = std::min(borderWidth + dy, (float)(MAX_BORDER_WIDTH));
	}
	else if (dy < 0)
	{
		borderWidth = std::max(borderWidth + dy, (float)(MIN_BORDER_WIDTH));
	}

	// ...
}

void Application::OnFileChanged(const char *filename)
{
	Shader::ReloadSingleShader(filename);
}
