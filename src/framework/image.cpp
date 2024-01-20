#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "GL/glew.h"
#include "../extra/picopng.h"
#include "image.h"
#include "utils.h"
#include "camera.h"
#include "mesh.h"

Image::Image()
{
	width = 0;
	height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width * height];
	memset(pixels, 0, width * height * sizeof(Color));
}

// Copy constructor
Image::Image(const Image &c)
{
	pixels = NULL;
	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;
	if (c.pixels)
	{
		pixels = new Color[width * height];
		memcpy(pixels, c.pixels, width * height * bytes_per_pixel);
	}
}

// Assign operator
Image &Image::operator=(const Image &c)
{
	if (pixels)
		delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;

	if (c.pixels)
	{
		pixels = new Color[width * height * bytes_per_pixel];
		memcpy(pixels, c.pixels, width * height * bytes_per_pixel);
	}
	return *this;
}

Image::~Image()
{
	if (pixels)
		delete pixels;
}

void Image::Render()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(width, height, bytes_per_pixel == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

// Change image size (the old one will remain in the top-left corner)
void Image::Resize(unsigned int width, unsigned int height)
{
	Color *new_pixels = new Color[width * height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for (unsigned int x = 0; x < min_width; ++x)
		for (unsigned int y = 0; y < min_height; ++y)
			new_pixels[y * width + x] = GetPixel(x, y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

// Change image size and scale the content
void Image::Scale(unsigned int width, unsigned int height)
{
	Color *new_pixels = new Color[width * height];

	for (unsigned int x = 0; x < width; ++x)
		for (unsigned int y = 0; y < height; ++y)
			new_pixels[y * width + x] = GetPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)));

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::GetArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for (unsigned int x = 0; x < width; ++x)
		for (unsigned int y = 0; y < height; ++x)
		{
			if ((x + start_x) < this->width && (y + start_y) < this->height)
				result.SetPixel(x, y, GetPixel(x + start_x, y + start_y));
		}
	return result;
}

void Image::FlipY()
{
	int row_size = bytes_per_pixel * width;
	Uint8 *temp_row = new Uint8[row_size];
#pragma omp simd
	for (int y = 0; y < height * 0.5; y += 1)
	{
		Uint8 *pos = (Uint8 *)pixels + y * row_size;
		memcpy(temp_row, pos, row_size);
		Uint8 *pos2 = (Uint8 *)pixels + (height - y - 1) * row_size;
		memcpy(pos, pos2, row_size);
		memcpy(pos2, temp_row, row_size);
	}
	delete[] temp_row;
}

bool Image::LoadPNG(const char *filename, bool flip_y)
{
	std::string sfullPath = absResPath(filename);
	std::ifstream file(sfullPath, std::ios::in | std::ios::binary | std::ios::ate);

	// Get filesize
	std::streamsize size = 0;
	if (file.seekg(0, std::ios::end).good())
		size = file.tellg();
	if (file.seekg(0, std::ios::beg).good())
		size -= file.tellg();

	if (!size)
		return false;

	std::vector<unsigned char> buffer;

	// Read contents of the file into the vector
	if (size > 0)
	{
		buffer.resize((size_t)size);
		file.read((char *)(&buffer[0]), size);
	}
	else
		buffer.clear();

	std::vector<unsigned char> out_image;

	if (decodePNG(out_image, width, height, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size(), true) != 0)
		return false;

	size_t bufferSize = out_image.size();
	unsigned int originalBytesPerPixel = (unsigned int)bufferSize / (width * height);

	// Force 3 channels
	bytes_per_pixel = 3;

	if (originalBytesPerPixel == 3)
	{
		pixels = new Color[bufferSize];
		memcpy(pixels, &out_image[0], bufferSize);
	}
	else if (originalBytesPerPixel == 4)
	{

		unsigned int newBufferSize = width * height * bytes_per_pixel;
		pixels = new Color[newBufferSize];

		unsigned int k = 0;
		for (unsigned int i = 0; i < bufferSize; i += originalBytesPerPixel)
		{
			pixels[k] = Color(out_image[i], out_image[i + 1], out_image[i + 2]);
			k++;
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	return true;
}

// Loads an image from a TGA file
bool Image::LoadTGA(const char *filename, bool flip_y)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int imageSize;
	unsigned int bytesPerPixel;

	std::string sfullPath = absResPath(filename);

	FILE *file = fopen(sfullPath.c_str(), "rb");
	if (file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "File not found: " << sfullPath.c_str() << std::endl;
		if (file == NULL)
			return NULL;
		else
		{
			fclose(file);
			return NULL;
		}
	}

	TGAInfo *tgainfo = new TGAInfo;

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

	tgainfo->data = new unsigned char[imageSize];

	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		if (tgainfo->data != NULL)
			delete tgainfo->data;

		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	// Save info in image
	if (pixels)
		delete pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width * height];

	// Convert to float all pixels
	for (unsigned int y = 0; y < height; ++y)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			// Make sure we don't access out of memory
			if ((pos < imageSize) && (pos + 1 < imageSize) && (pos + 2 < imageSize))
				SetPixel(x, height - y - 1, Color(tgainfo->data[pos + 2], tgainfo->data[pos + 1], tgainfo->data[pos]));
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	delete tgainfo->data;
	delete tgainfo;

	return true;
}

// Saves the image to a TGA file
bool Image::SaveTGA(const char *filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	std::string fullPath = absResPath(filename);
	FILE *file = fopen(fullPath.c_str(), "wb");
	if (file == NULL)
	{
		perror("Failed to open file: ");
		return false;
	}

	unsigned short header_short[3];
	header_short[0] = width;
	header_short[1] = height;
	unsigned char *header = (unsigned char *)header_short;
	header[4] = 24;
	header[5] = 0;

	fwrite(TGAheader, 1, sizeof(TGAheader), file);
	fwrite(header, 1, 6, file);

	// Convert pixels to unsigned char
	unsigned char *bytes = new unsigned char[width * height * 3];
	for (unsigned int y = 0; y < height; ++y)
		for (unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[y * width + x];
			unsigned int pos = (y * width + x) * 3;
			bytes[pos + 2] = c.r;
			bytes[pos + 1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width * height * 3, file);
	fclose(file);

	return true;
}

void Image::DrawLineDDA(int x0, int y0, int x1, int y1, const Color &c)
{
	int dx = x1 - x0; // Distancia en x
	int dy = y1 - y0; // Distancia en y

	int d = std::max(abs(dx), abs(dy)); // El mayor lado del triángulo

	float xInc = dx / (float)d; // Incremento de x para cada paso
	float yInc = dy / (float)d; // Incremento de y para cada paso

	// Podriamos utilizar std::vector, pero no es necesario en este contexto, ya que no vamos a añadir ni eliminar elementos.

	float x = x0; // Poner pixel para cada paso
	float y = y0; // Poner pixel para cada paso

	for (int i = 0; i <= d; ++i) // Loop para cada uno de los pasos en la distancia d
	{
		SetPixelSafe(floor(x), floor(y), c); // Ponermos el pixel en la posicion actual
		// SetPixel(floor(x), floor(y), c); // Ponermos el pixel en la posicion actual
		// Sumamos el incremento en cada uno de los ejes
		x += xInc;
		y += yInc;
	}
}

void Image::DrawRect(int x, int y, int w, int h, const Color &borderColor, int borderWidth, bool isFilled, const Color &fillColor)
{

	// Ajustar x e y si w o h son negativos, y convertir w y h a valores absolutos
	if (w < 0)
	{
		x += w + 1;
		w = -w;
	}
	if (h < 0)
	{
		y += h + 1;
		h = -h;
	}

	if (isFilled)
	{
		for (int i = 0; i < w; ++i)
		{
			for (int j = 0; j < h; ++j)
			{
				SetPixelSafe(x + i, y + j, fillColor);
			}
		}
	}

	// Se debe tener en cuenta que los bordes van a ser interiores, por lo que que el tamaño del rectangulo, cuando más grande sea el border más pequeño será el rectangulo

	for (int bw = 0; bw < borderWidth; ++bw)
	{
		// Si queremos cambiar los bordes a exteriores, tendremos que cambiar la operacion que implica bw
		for (int i = bw; i < w - bw; ++i)
		{
			SetPixelSafe(x + i, y + bw, borderColor);		  // Linea horizontal superior
			SetPixelSafe(x + i, y + h - 1 - bw, borderColor); // Linea horizontal inferior
		}

		for (int j = bw; j < h - bw; ++j)
		{
			SetPixelSafe(x + bw, y + j, borderColor);		  // Linea vertical izquierda
			SetPixelSafe(x + w - 1 - bw, y + j, borderColor); // Linea vertical derecha
		}
	}
}

void Image::DrawCircle(int x, int y, int r, const Color &borderColor,
					   int borderWidth, bool isFilled, const Color &fillColor)
{
	// Drawing the circumference
	for (int xOct = -r; xOct <= r; xOct++)
	{
		for (int yOct = -r; yOct <= r; yOct++)
		{
			if (xOct * xOct + yOct * yOct <= r * r)
			{
				if (isFilled)
				{
					// Pintar interior del círculo
					SetPixelSafe(x + xOct, y + yOct, fillColor);
				}
				if (xOct * xOct + yOct * yOct >= (r - borderWidth) * (r - borderWidth))
				{
					// Pintar pixel del borde
					SetPixelSafe(x + xOct, y + yOct, borderColor);
				}
			}
		}
	}
}

void Image::ScanLineDDA(int x0, int y0, int x1, int y1, std::vector<Cell> &table)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int d = std::max(abs(dx), abs(dy));

	float xInc = dx / (float)(d);
	float yInc = dy / (float)(d);

	float x = x0;
	float y = y0;

	for (int i = 0; i <= d; i++)
	{
		int ix = (int)(std::floor(x));
		int iy = (int)(std::floor(y));

		// Nos aseguramos de no acceder a posiciones fuera de la tabla
		if (iy >= 0 && iy < table.size())
		{
			table[iy].minx = std::min(table[iy].minx, ix);
			table[iy].maxx = std::max(table[iy].maxx, ix);
		}

		x += xInc;
		y += yInc;
	}
}

void Image::DrawTriangle(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Color &borderColor, bool isFilled, const Color &fillColor)
{
	// FIXME: Modifiy to adapt to the AET Triangle drawing algorithm
	std::vector<Cell> table(height);

	ScanLineDDA(p0.x, p0.y, p1.x, p1.y, table);
	ScanLineDDA(p1.x, p1.y, p2.x, p2.y, table);
	ScanLineDDA(p2.x, p2.y, p0.x, p0.y, table);

	// Llenamos el triángulo si es necesario
	if (isFilled)
	{
		for (int y = 0; y < table.size(); y++)
		{
			if (table[y].minx <= table[y].maxx)
			{
				for (int x = table[y].minx; x <= table[y].maxx; x++)
				{
					SetPixel(x, y, fillColor);
				}
			}
		}
	}

	// Dibuja el borde del triángulo
	DrawLineDDA(p0.x, p0.y, p1.x, p1.y, borderColor);
	DrawLineDDA(p1.x, p1.y, p2.x, p2.y, borderColor);
	DrawLineDDA(p2.x, p2.y, p0.x, p0.y, borderColor);
}

void Image::DrawImage(const Image &image, int x, int y, bool top)
{
	// Calcular la coordenada Y de inicio basándose en el parámetro 'top'
	int startY = top ? y : (this->height - y - image.height);

	// Recorrer cada píxel de la imagen de entrada
	for (int iy = 0; iy < image.height; iy++)
	{
		for (int ix = 0; ix < image.width; ix++)
		{
			// Obtener el color del píxel actual de la imagen de entrada
			Color color = image.GetPixel(ix, iy);

			// Calcular las coordenadas en el framebuffer
			int destX = x + ix;
			int destY = startY + iy;

			// Establecer el píxel en el framebuffer, si está dentro de los límites
			if (destX >= 0 && destX < this->width && destY >= 0 && destY < this->height)
			{
				this->SetPixel(destX, destY, color);
			}
		}
	}
}

#ifndef IGNORE_LAMBDAS

// You can apply and algorithm for two images and store the result in the first one
// ForEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void ForEachPixel(Image &img, const Image &img2, F f)
{
	for (unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f(img.pixels[pos], img2.pixels[pos]);
}

#endif

FloatImage::FloatImage(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new float[width * height];
	memset(pixels, 0, width * height * sizeof(float));
}

// Copy constructor
FloatImage::FloatImage(const FloatImage &c)
{
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
}

// Assign operator
FloatImage &FloatImage::operator=(const FloatImage &c)
{
	if (pixels)
		delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height * sizeof(float)];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
	return *this;
}

FloatImage::~FloatImage()
{
	if (pixels)
		delete pixels;
}

// Change image size (the old one will remain in the top-left corner)
void FloatImage::Resize(unsigned int width, unsigned int height)
{
	float *new_pixels = new float[width * height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for (unsigned int x = 0; x < min_width; ++x)
		for (unsigned int y = 0; y < min_height; ++y)
			new_pixels[y * width + x] = GetPixel(x, y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}