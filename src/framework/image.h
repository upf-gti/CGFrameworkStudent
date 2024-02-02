/*
	+ This file defines the class Image that allows to manipulate images.
	+ It defines all the need operators for Color and Image
*/

#pragma once

#include <string.h>
#include <stdio.h>
#include <iostream>
#include "framework.h"

// remove unsafe warnings
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#endif

class FloatImage;
class Entity;
class Camera;

// A matrix of pixels
class Image
{
	// A general struct to store all the information about a TGA file
	typedef struct sTGAInfo
	{
		unsigned int width;
		unsigned int height;
		unsigned int bpp;	 // Bits per pixel
		unsigned char *data; // Bytes with the pixel information
	} TGAInfo;

	// LAB 1 : Cell struct
	typedef struct Cell
	{
		int minx = INT_MAX;
		int maxx = INT_MIN;
	} Cell;
	struct sTriangleInfo
	{
		Vector3 vertices[3]; // 3 vertices of the triangle
		Vector2 uvs[3];		 // UV coordinates for each vertex
		Color colors[3];	 // Color at each vertex
		Image *texture;		 // Pointer to an optional texture image

		sTriangleInfo(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2,
					  const Color &c0, const Color &c1, const Color &c2,
					  Image *tex, const Vector2 &uv0, const Vector2 &uv1, const Vector2 &uv2)
			: texture(tex)
		{
			vertices[0] = v0;
			vertices[1] = v1;
			vertices[2] = v2;
			colors[0] = c0;
			colors[1] = c1;
			colors[2] = c2;
			uvs[0] = uv0;
			uvs[1] = uv1;
			uvs[2] = uv2;
		}
	};

public:
	unsigned int width;
	unsigned int height;
	unsigned int bytes_per_pixel = 3; // Bits per pixel

	Color *pixels;

	// Constructors
	Image();
	Image(unsigned int width, unsigned int height);
	Image(const Image &c);
	Image &operator=(const Image &c); // Assign operator

	// Destructor
	~Image();

	void Render();

	// Get the pixel at position x,y
	Color GetPixel(unsigned int x, unsigned int y) const { return pixels[y * width + x]; }
	Color &GetPixelRef(unsigned int x, unsigned int y) { return pixels[y * width + x]; }
	Color GetPixelSafe(unsigned int x, unsigned int y) const
	{
		x = clamp((unsigned int)x, 0, width - 1);
		y = clamp((unsigned int)y, 0, height - 1);
		return pixels[y * width + x];
	}

	// Set the pixel at position x,y with value C
	void SetPixel(unsigned int x, unsigned int y, const Color &c) { pixels[y * width + x] = c; }
	void SetPixelSafe(unsigned int x, unsigned int y, const Color &c) const
	{
		if (x < 0 || x > width - 1)
			return;
		if (y < 0 || y > height - 1)
			return;
		pixels[y * width + x] = c;
	}

	void Resize(unsigned int width, unsigned int height);
	void Scale(unsigned int width, unsigned int height);

	void FlipY(); // Flip the image top-down

	// Fill the image with the color C
	void Fill(const Color &c)
	{
		for (unsigned int pos = 0; pos < width * height; ++pos)
			pixels[pos] = c;
	}

	// Returns a new image with the area from (startx,starty) of size width,height
	Image GetArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height);

	// Save or load images from the hard drive
	bool LoadPNG(const char *filename, bool flip_y = true);
	bool LoadTGA(const char *filename, bool flip_y = false);
	bool SaveTGA(const char *filename);

	// LAB1: Dibujar linea de (x0,y0) a (x1,y1)
	void DrawLineDDA(int x0, int y0, int x1, int y1, const Color &c);

	// LAB1: Dibujar rectangulo de (x,y) a (x+w,y+h) (Update)
	void DrawRect(int x, int y, int w, int h, const Color &borderColor, int borderWidth, bool isFilled, const Color &fillColor);

	// LAB1: Dibujar circulo con centro en (x,y) y radio
	void DrawCircle(int x, int y, int r, const Color &borderColor, int borderWidth, bool isFilled, const Color &fillColor);

	// LAB1: Almacena en table los valores de x para cada valor de y entre y0 e y1
	void ScanLineDDA(int x0, int y0, int x1, int y1, std::vector<Cell> &table);
	// LAB1: Dibujar triangulo con vertices en p0, p1, p2
	void DrawTriangle(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Color &borderColor, bool isFilled, const Color &fillColor);

	// LAB3: Dibujar triangulo Interpolado con vertices en p0, p1, p2
	// TODO: Extra. Compress the parameters into an struct (already created on top, just needs to be added to the function signature)

	/*called as sTriangleInfo triangleInfo(v0, v1, v2, c0, c1, c2, texture, uv0, uv1, uv2);
	image.DrawTriangleInterpolated(triangleInfo, zbuffer);*/

	// void DrawTriangleInterpolated(const sTriangleInfo& triangle, FloatImage* zbuffer = nullptr);

	// void DrawTriangleInterpolated(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Color &c0, const Color &c1, const Color &c2, FloatImage *zbuffer = nullptr, Image *texture = nullptr, const Vector2 &uv0, const Vector2 &uv1, const Vector2 &uv2);

	// LAB1: Rasterizar una imagen por pantalla
	void DrawImage(const Image &image, int x, int y, bool top);

// Used to easy code
#ifndef IGNORE_LAMBDAS

	// Applies an algorithm to every pixel in an image
	// you can use lambda sintax:   img.forEachPixel( [](Color c) { return c*2; });
	// or callback sintax:   img.forEachPixel( mycallback ); //the callback has to be Color mycallback(Color c) { ... }
	template <typename F>
	Image &ForEachPixel(F callback)
	{
		for (unsigned int pos = 0; pos < width * height; ++pos)
			pixels[pos] = callback(pixels[pos]);
		return *this;
	}
#endif
};

// Image storing one float per pixel instead of a 3 or 4 component Color

class FloatImage
{
public:
	unsigned int width;
	unsigned int height;
	float *pixels;

	// CONSTRUCTORS
	FloatImage()
	{
		width = height = 0;
		pixels = NULL;
	}
	FloatImage(unsigned int width, unsigned int height);
	FloatImage(const FloatImage &c);
	FloatImage &operator=(const FloatImage &c); // assign operator

	// destructor
	~FloatImage();

	void Fill(const float &v)
	{
		for (unsigned int pos = 0; pos < width * height; ++pos)
			pixels[pos] = v;
	}

	// get the pixel at position x,y
	float GetPixel(unsigned int x, unsigned int y) const { return pixels[y * width + x]; }
	float &GetPixelRef(unsigned int x, unsigned int y) { return pixels[y * width + x]; }

	// set the pixel at position x,y with value C
	inline void SetPixel(unsigned int x, unsigned int y, const float &v) { pixels[y * width + x] = v; }

	void Resize(unsigned int width, unsigned int height);
};
