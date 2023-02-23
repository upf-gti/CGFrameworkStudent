/*
	This file has all the includes so the app works in different systems.
	It is a little bit low level so do not worry about the code.
*/

#pragma once

#include <SDL.h>
#include <SDL_syswm.h>
#include "GL/glew.h"

// GL
#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glext.h>
    #include <OpenGL/glu.h>

    #include <SDL_opengl.h>
#else
    #include <SDL_opengl.h>

    #include <GL/glu.h>
#endif

#include <iostream>
#include <cmath>
#include <cassert>
#include <string.h>
