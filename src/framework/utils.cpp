#include "utils.h"
#include "GL/glew.h"

#ifdef WIN32
	#include <windows.h>
    #include <codecvt>

    #define PATH_MAX 256

    #include "../extra/directory_watcher.h"

    CDirectoryWatcher dir_watcher_data;

#else
	#include <sys/time.h>

#if defined(__linux__)
	#include <limits.h>
	#include <unistd.h>
#elif defined(__APPLE__)
    #include <sys/param.h>
    #include <mach-o/dyld.h>
#endif
#endif

#include "main/includes.h"
#include "application.h"
#include "image.h"

std::string absResPath( const std::string& p_sFile )
{
	std::string sFullPath;
	std::string sFileName;
	std::string sFixedPath = std::string("../../res/") + p_sFile;

#if defined(WIN32)
	char result[PATH_MAX];
	GetModuleFileName( NULL, (LPSTR)result, PATH_MAX);
	sFileName = '\\' + sFixedPath;
	sFullPath = result;
#elif defined(__linux__)
	char result[PATH_MAX];
	ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
	sFullPath = std::string( result, ( count > 0 ) ? count : 0 );
	sFileName = '/' + sFixedPath;
#elif defined(__APPLE__)
	char result[PATH_MAX];
	uint32_t bufsize = PATH_MAX;
	if( !_NSGetExecutablePath( result, &bufsize ) )
		puts( result );
	sFullPath = &result[0];
	sFileName = '/' + sFixedPath;
#endif
	return sFullPath.substr( 0, sFullPath.find_last_of( "\\/" ) ) + sFileName;
}

// This function is used to access OpenGL Extensions (special features not supported by all cards)
void* getGLProcAddress(const char* name)
{
	return SDL_GL_GetProcAddress(name);
}

bool checkGLErrors()
{
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL Error: " << errString << std::endl;
		return false;
	}

	return true;
}

// Create a window using SDL
SDL_Window* createWindow(const char* caption, int width, int height )
{
	int bpp = 0;

	SDL_Init(SDL_INIT_EVERYTHING);

	// Set attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); // Or 24

//	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// Antialiasing (disable this lines if it goes too slow)
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4 ); //increase to have smoother polygons

	// Create the window
	SDL_Window *window = SDL_CreateWindow(
		caption, 100, 100, width, height, 
		SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

	if(!window)
	{
		fprintf(stderr, "Window creation error: %s\n", SDL_GetError());
		exit(-1);
	}
  
	SDL_SysWMinfo  wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(window, &wmInfo);

#ifdef WIN32
	HWND hwnd = wmInfo.info.win.window;
	std::string sShadersPath = absResPath("shaders");
	dir_watcher_data.start(sShadersPath.c_str(), hwnd);
#endif

	// Create an OpenGL context associated with the window.
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);

	glewExperimental = true; // Se necesita en el perfil de base.
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Error initializing GLEW\n");
		exit(-1);
	}

	// Set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// In case of exit...
	atexit(SDL_Quit);

	// Get events from the queue of unprocessed events
	SDL_PumpEvents(); //without this line asserts could fail on windows

	return window;
}

// The application main loop
void launchLoop(Application* app)
{
	SDL_Event sdlEvent;
	Uint32 last_time = SDL_GetTicks();
	int x,y;

	SDL_GetMouseState(&x,&y);
	app->mouse_position.set(static_cast<float>(x), static_cast<float>(y));

	Uint32 start_time = SDL_GetTicks();

	// Infinite loop
	while (1)
	{
		// Read keyboard state and stored in keystate
		app->keystate = SDL_GetKeyboardState(NULL);

		// Clear the window and the depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render frame
		app->Render();

		// Swap between front buffer and back buffer
		SDL_GL_SwapWindow(app->window);

		// Update events
		while(SDL_PollEvent(&sdlEvent))
		{
			switch(sdlEvent.type)
				{
					case SDL_QUIT: return; break; // EVENT for when the user clicks the [x] in the corner
					case SDL_MOUSEBUTTONDOWN: // EXAMPLE OF sync mouse input
						app->OnMouseButtonDown(sdlEvent.button);
						break;
					case SDL_MOUSEBUTTONUP:
						app->OnMouseButtonUp(sdlEvent.button);
						break;
					case SDL_MOUSEMOTION:
						app->OnMouseMove(sdlEvent.button);
						break;
					case SDL_KEYUP:  // EXAMPLE OF sync keyboard input
						app->OnKeyPressed(sdlEvent.key);
						break;
					case SDL_MOUSEWHEEL:
						app->OnWheel(sdlEvent.wheel);
						break;
					case SDL_WINDOWEVENT:
						switch (sdlEvent.window.event) {
							case SDL_WINDOWEVENT_RESIZED: // Resize OpenGL context
								std::cout << "window resize" << std::endl;
								app->SetWindowSize( sdlEvent.window.data1, sdlEvent.window.data2 );
								break;
						}
						break;
#ifdef WIN32
					case CDirectoryWatcher::WM_FILE_CHANGED:
						const char* filename = (const char*)(dir_watcher_data.file_name);
						app->OnFileChanged(filename);
						break;
#endif
				}
		}

		// Get mouse position and delta
		app->mouse_state = SDL_GetMouseState(&x,&y);
		app->mouse_delta.set( app->mouse_position.x - x, app->window_height - app->mouse_position.y - y );
		app->mouse_position.set(static_cast<float>(x), static_cast<float>(app->window_height - y));

		// Update logic
		Uint32 now = SDL_GetTicks();
		float elapsed_time = (now - last_time) * 0.001f; // 0.001 converts from milliseconds to seconds
		app->time = (now - start_time) * 0.001f;
		app->Update(elapsed_time);
		last_time = now;

		// Check errors in opengl only when working in debug
		#ifdef _DEBUG
			checkGLErrors();
		#endif
	}

	return;
}

std::vector<std::string> tokenize(const std::string& source, const char* delimiters, bool process_strings)
{
	std::vector<std::string> tokens;

	std::string str;
	char del_size = (char)strlen(delimiters);
	const char* pos = source.c_str();
	char in_string = 0;
	int i = 0;
	while (*pos != 0)
	{
		bool split = false;

		if (!process_strings || (process_strings && in_string == 0))
		{
			for (i = 0; i < del_size && *pos != delimiters[i]; i++);
			if (i != del_size) split = true;
		}

		if (process_strings && (*pos == '\"' || *pos == '\''))
		{
			if (!str.empty() && in_string == 0) //some chars remaining
			{
				tokens.push_back(str);
				str.clear();
			}

			in_string = (in_string != 0 ? 0 : *pos);
			if (in_string == 0)
			{
				str += *pos;
				split = true;
			}
		}

		if (split)
		{
			if (!str.empty())
			{
				tokens.push_back(str);
				str.clear();
			}
		}
		else
			str += *pos;
		pos++;
	}
	if (!str.empty())
		tokens.push_back(str);
	return tokens;
}



Vector2 parseVector2(const char* text)
{
	int pos = 0;
	char num[255];
	const char* start = text;
	const char* current = text;
	Vector2 result;

	while (1)
	{
		if (*current == ',' || (*current == '\0' && current != text))
		{
			strncpy(num, start, current - start);
			num[current - start] = '\0';
			start = current + 1;
			switch (pos)
			{
			case 0: result.x = (float)atof(num); break;
			case 1: result.y = (float)atof(num); break;
			default: return result; break;
			}
			++pos;
			if (*current == '\0')
				break;
		}

		++current;
	}
	return result;
}

Vector3 parseVector3(const char* text, const char separator)
{
	int pos = 0;
	char num[255];
	const char* start = text;
	const char* current = text;
	Vector3 result;

	while (1)
	{
		if (*current == separator || (*current == '\0' && current != text))
		{
			strncpy(num, start, current - start);
			num[current - start] = '\0';
			start = current + 1;
			if (num[0] != 'x') //
				switch (pos)
				{
				case 0: result.x = (float)atof(num); break;
				case 1: result.y = (float)atof(num); break;
				case 2: result.z = (float)atof(num); break;
				default: return result; break;
				}

			++pos;
			if (*current == '\0')
				break;
		}

		++current;
	}

	return result;
};
