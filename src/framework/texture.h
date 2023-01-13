/*  
	This contains a texture wrapper to use textures
*/

#pragma once

#include "main/includes.h"
#include <map>
#include <string>

class Texture
{
	typedef struct sTGAInfo //a general struct to store all the information about a TGA file
	{
		GLuint width;
		GLuint height;
		GLuint bpp; //bits per pixel
		GLubyte* data; //bytes with the pixel information
	} TGAInfo;

public:

	GLuint texture_id; // GL id to identify the texture in opengl, every texture must have its own id
	std::string filename;

	float width;
	float height;

	unsigned int format; //GL_RGB, GL_RGBA
	unsigned int type; //GL_UNSIGNED_INT, GL_FLOAT

	bool mipmaps;
	unsigned int wrapS;
	unsigned int wrapT;

	Texture();
	void Create(unsigned int width, unsigned int height, unsigned int format = GL_RGB, unsigned int type = GL_UNSIGNED_BYTE, bool mipmaps = true, Uint8* data = NULL, unsigned int internal_format = 0, unsigned int wrap = GL_CLAMP_TO_EDGE);
	void Bind();
	void Unbind();
	void Clear();
	static void UnbindAll();

	void Upload(unsigned int format, unsigned int type, bool mipmaps, Uint8* data, unsigned int internal_format = 0);
	bool Load(const char* filename, bool mipmaps = true);
	void GenerateMipmaps();

	static Texture* Get(const char* filename);
	static std::map<std::string, Texture*> s_Textures;

protected:
	TGAInfo* LoadTGA(const char* filename);
};