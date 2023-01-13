/*
	This file has all the includes so the app works in different systems.
	It is a little bit low level so do not worry about the code.
*/

#pragma once

#include <SDL.h>
#include <SDL_syswm.h>

// GL
#ifdef __APPLE__
    #include <GL/glew.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glext.h>
    #include <OpenGL/glu.h>

    #include <SDL_opengl.h>
#else
    #define glActiveTexture glActiveTexture___
    #include <SDL_opengl.h>
    #undef glActiveTexture

    #include <GL/glu.h>
#endif

#include <iostream>
#include <cmath>
#include <cassert>
#include <string.h>

// Used to access opengl extensions
//void* getGLProcAddress(const char*);
#define REGISTER_GLEXT(RET, FUNCNAME, ...) typedef RET (APIENTRY * FUNCNAME ## _func)(__VA_ARGS__); FUNCNAME ## _func FUNCNAME = NULL; 
#define IMPORT_GLEXT(FUNCNAME) FUNCNAME = (FUNCNAME ## _func) SDL_GL_GetProcAddress(#FUNCNAME); if (FUNCNAME == NULL) { std::cout << "ERROR: This Graphics card doesnt support " << #FUNCNAME << std::endl; }
