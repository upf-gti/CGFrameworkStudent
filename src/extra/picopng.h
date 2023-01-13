#pragma once

#include <vector>
#include <string.h>

int decodePNG(std::vector<unsigned char>& out_image, unsigned int& image_width, unsigned int& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);