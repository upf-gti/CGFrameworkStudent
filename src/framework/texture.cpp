#include "texture.h"
#include "utils.h"
#include "image.h"

#include <iostream> //to output
#include <cmath>

std::map<std::string, Texture*> Texture::s_Textures;

Texture::Texture()
{
	width = 0;
	height = 0;
	format = GL_RGB;
	wrapS = wrapT = GL_CLAMP_TO_EDGE;
	mipmaps = false;
	type = GL_UNSIGNED_BYTE;
}

Texture* Texture::Get(const char* filename)
{
	std::string name = std::string(filename);
	std::map<std::string, Texture*>::iterator it = s_Textures.find(name);
	if (it != s_Textures.end())
		return it->second;

	Texture* texture = new Texture();
	if (!texture->Load(filename))
		return NULL;
	s_Textures[name] = texture;
	return texture;
}

void Texture::Create(unsigned int width, unsigned int height, unsigned int format, unsigned int type, bool mipmaps, Uint8* data, unsigned int internal_format, unsigned int wrap)
{
	this->width = (float)width;
	this->height = (float)height;
	this->format = format;
	this->type = type;
	this->mipmaps = mipmaps && isPowerOfTwo(width) && isPowerOfTwo(height) && format != GL_DEPTH_COMPONENT;
	this->wrapS = this->wrapT = wrap;

	//Delete previous texture and ensure that previous bounded texture_id is not of another texture type
	if (this->texture_id != 0)
		Clear();

	if (texture_id == 0)
		glGenTextures(1, &texture_id); //we need to create an unique ID for the texture

	if (!checkGLErrors()) {
		throw("Error creating texture");
	}

	if (data != NULL)
		Upload(this->format, this->type, this->mipmaps, data, internal_format);
}

//uploads the bytes of a texture to the VRAM
void Texture::Upload(unsigned int format, unsigned int type, bool mipmaps, Uint8* data, unsigned int internal_format)
{
	glBindTexture(GL_TEXTURE_2D, texture_id);	// We activate this id to tell opengl we are going to use this texture

	glTexImage2D(GL_TEXTURE_2D, 0, internal_format == 0 ? format : internal_format, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, format, type, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//set the mag filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST); //set the min filter
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

	if (data && this->mipmaps)
		GenerateMipmaps();

	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::Load(const char* filename, bool mipmaps)
{
	std::string sfullPath = absResPath(filename);
	std::string ext = sfullPath.substr(sfullPath.size() - 4,4 );

	if (ext == ".tga" || ext == ".TGA") {
		TGAInfo* tgainfo = LoadTGA(sfullPath.c_str());
		if (tgainfo == NULL)
			return false;

		this->filename = sfullPath;
		Create(tgainfo->width, tgainfo->height, tgainfo->bpp == 24 ? GL_BGR : GL_BGRA, GL_UNSIGNED_BYTE, mipmaps, tgainfo->data, (tgainfo->bpp == 24 ? 3 : 4));

		delete tgainfo->data;
		delete tgainfo;
		return true;
	}
	else if (ext == ".png" || ext == ".PNG") {
		Image* image = new Image();
		if (!image->LoadPNG(filename)) {
			delete image;
			return false;
		}
		this->filename = sfullPath;
		Create(image->width, image->height, image->bytes_per_pixel == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, mipmaps, (Uint8*)image->pixels);
		return true;
	}
	else {
		std::cout << "unsupported texture format: " << ext << " only TGA supported" << std::endl;
		exit(1);
	}

	return false;
}

void Texture::Bind()
{
	glEnable( GL_TEXTURE_2D ); //enable the textures 
	glBindTexture( GL_TEXTURE_2D, texture_id );	//enable the id of the texture we are going to use
}

void Texture::Unbind()
{
	glDisable( GL_TEXTURE_2D ); //disable the textures 
	glBindTexture( GL_TEXTURE_2D, 0 );	//disable the id of the texture we are going to use
}

void Texture::Clear()
{
	Unbind();
	glDeleteTextures(1, &texture_id);
	texture_id = 0;
}

void Texture::UnbindAll()
{
	glDisable( GL_TEXTURE_CUBE_MAP );
	glDisable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, 0 );
	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
}

void Texture::GenerateMipmaps()
{
	if (glGenerateMipmapEXT) //extension of GL3.0 (I guess faster)
	{
		glBindTexture(GL_TEXTURE_2D, texture_id);	//enable the id of the texture we are going to use
		glGenerateMipmapEXT(GL_TEXTURE_2D);
	}
	else {
		// Not using any older method
	}
}

Texture::TGAInfo* Texture::LoadTGA(const char* filename)
{
    GLubyte TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    GLubyte TGAcompare[12];
    GLubyte header[6];
    GLuint bytesPerPixel;
    GLuint imageSize;
    GLuint type = GL_RGBA;

    FILE * file = fopen(filename, "rb");
    
    if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
        memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
        fread(header, 1, sizeof(header), file) != sizeof(header))
    {
        if (file == NULL)
            return NULL;
        else
        {
            fclose(file);
            return NULL;
        }
    }

	TGAInfo* tgainfo = new TGAInfo;
    
    tgainfo->width = header[1] * 256 + header[0];
    tgainfo->height = header[3] * 256 + header[2];
    
    if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
    {
        fclose(file);
		delete tgainfo;
        return NULL;
    }
    
    tgainfo->bpp = header[4];
    bytesPerPixel = tgainfo->bpp / 8;
    imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
    tgainfo->data = (GLubyte*)malloc(imageSize);
    
    if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
    {
        if (tgainfo->data != NULL)
            free(tgainfo->data);
            
        fclose(file);
		delete tgainfo;
        return NULL;
    }
    
    fclose(file);

	return tgainfo;
}
