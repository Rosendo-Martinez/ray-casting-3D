#ifndef HEIGHT_MAP_H
#define HEIGHT_MAP_H


class HeightMap
{
public:
    HeightMap();
    bool valid();
    void load(const char * filename);
    float operator()(float u, float v);
    ~HeightMap();

private:
    unsigned char* image = nullptr;
    int x;
    int y; 
    int n;
};

#endif
