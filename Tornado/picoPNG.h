#pragma once

#include <vector>
#include <string>

int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);
void loadFile(std::vector<unsigned char>& buffer, const std::string& filename); //designed for loading files from hard disk in an std::vector