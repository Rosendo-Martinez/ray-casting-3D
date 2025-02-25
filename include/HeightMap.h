#ifndef HEIGHT_MAP_H
#define HEIGHT_MAP_H

#include "Vector2f.h"

class HeightMap
{
public:
    HeightMap();
    bool valid();
    void load(const char * filename);
    Vector2f getGradient(float u, float v);
    ~HeightMap();

private:
    unsigned char* image;
    int scanline_width;
    int scanline_count;

    float getHeight(int index);
    int pixelIndex(float u, float v);
};

#endif
