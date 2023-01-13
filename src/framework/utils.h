/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This contains several functions that can be useful when programming your game.
*/

#pragma once

#include "framework.h"
#include "SDL.h"
#include <string>

//General functions **************
class Application;
class Image;

//check opengl errors
bool checkGLErrors();

SDL_Window* createWindow(const char* caption, int width, int height);
void launchLoop(Application* app);

//fast random generator
inline unsigned long frand(void) {          //period 2^96-1
	unsigned long t;
	static unsigned long x = 123456789, y = 362436069, z = 521288629;
	x ^= x << 16; x ^= x >> 5; x ^= x << 1;
	t = x; x = y;y = z;
	z = t ^ x ^ y;
	return z;
}

inline bool isPowerOfTwo(int n) { return (n & (n - 1)) == 0; }
inline float randomValue() { return (frand() % 10000) / 10000.0f; }
std::string absResPath(const std::string& p_sFile);
std::vector<std::string> tokenize(const std::string& source, const char* delimiters, bool process_strings = false);
Vector2 parseVector2(const char* text);
Vector3 parseVector3(const char* text, const char separator);