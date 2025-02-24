#include "HeightMap.h"
#include "iostream"
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

HeightMap::HeightMap()
{}

bool HeightMap::valid()
{
    return this->image != nullptr;
}

/**
 * Assumes image is grey-scale. 8-bit. 
 */
void HeightMap::load(const char * filename)
{
    // First pixel (0,0) is top left
    this->image = stbi_load(filename, &x, &y, &n, 1); // force 1 channel per pixel (grayscale)

    if (this->image == nullptr)
    {
        std::cout << "Error: could not load: " << filename << '\n';
    }
}

float HeightMap::operator()(float u, float v)
{
    assert(u >= 0.0f && u <= 1.0f);
    assert(v >= 0.0f && v <= 1.0f);

    // NOTE: I'm not sure if this is correct way to map UV to pixel!
    int pixel_x = (int) u * this->x;
    int pixel_y = (int) (1 - v) * this->y;

    assert((pixel_y * x) + pixel_x < x * y);
    float grey = image[(pixel_y * x) + pixel_x];

    return grey;
}

HeightMap::~HeightMap()
{
    stbi_image_free(this->image);
}