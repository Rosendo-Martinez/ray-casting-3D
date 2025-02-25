#include "HeightMap.h"
#include "iostream"
#include <cassert>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

HeightMap::HeightMap()
    : image(nullptr), scanline_width(0), scanline_count(0)
{
}

HeightMap::~HeightMap()
{
    if (this->image != nullptr)
    {
        stbi_image_free(this->image);
    }
}

bool HeightMap::valid()
{
    return this->image != nullptr;
}

/**
 * Assumes image is grey-scale and 8-bit. 
 */
void HeightMap::load(const char * filename)
{
    int channels;

    // NOTE: First pixel (0,0) is top left.
    // NOTE: Force 1 channel per pixel for grayscale image.
    // IDEA: could process this into something easier to work with
    this->image = stbi_load(filename, &scanline_width, &scanline_count, &channels, 1);

    if (this->image == nullptr)
    {
        std::cout << "HeightMap: Error: could not load: " << filename << '\n';
    }
}

Vector2f HeightMap::getGradient(float u, float v)
{
    int center_pixel = pixelIndex(u,v);
    int top_pixel = center_pixel - scanline_width;
    int bottom_pixel = center_pixel + scanline_width;
    int left_pixel = center_pixel - 1;
    int right_pixel = center_pixel + 1;

    int pixel_count = scanline_width * scanline_count;

    // Inner pixel
    if (
        top_pixel >= 0 &&
        bottom_pixel <= pixel_count &&
        left_pixel >= 0 &&
        right_pixel <= pixel_count
    )
    {
        // Central finite difference
        float gradient_y = getHeight(bottom_pixel) - getHeight(top_pixel); // [-1, 1]
        float gradient_x = getHeight(left_pixel) - getHeight(right_pixel); // [-1, 1]

        assert(gradient_y >= -1.0f && gradient_y <= 1.0f);
        assert(gradient_x >= -1.0f && gradient_x <= 1.0f);

        return Vector2f(gradient_x, gradient_y);
    }
    
    // NOTE: Outer pixels will have zero gradient.
    //       Technically, they could have a gradient, and it could be 
    //       calculated using forward/backwards finite differences, BUT
    //       fuck that. Its too hard/complicated (I'm lazy). 
    //       For now, this is good enough!

    return Vector2f(0.0f);
}

/**
 * Returns height of pixel normalized to [0, 1].
 */
float HeightMap::getHeight(int index)
{
    return this->image[index] / 255.0f; // [0, 255] --> [0,1]
}

int HeightMap::pixelIndex(float u, float v)
{
    assert(u >= 0.0f && u <= 1.0f);
    assert(v >= 0.0f && v <= 1.0f);

    // NOTE: I'm not sure if this is correct way to map UV to pixel index!
    u = u * this->scanline_width;
    v = (1 - v) * this->scanline_count;
    int pixel_x = (int) u;
    int pixel_y = (int) v;

    int index = (pixel_y * this->scanline_width) + pixel_x;

    assert(index < scanline_width * scanline_count && index >= 0);
    return index;
}
