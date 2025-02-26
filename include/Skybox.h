#ifndef SKYBOX_H
#define SKYBOX_H

#include "Object3D.h"
#include "Rectangle.h"
#include <cassert>


class SkyBox
{
public:
    SkyBox();
    SkyBox(const Vector3f& center, float angle, Texture* front_tex, Texture* back_tex, Texture* right_tex, Texture* left_tex, Texture* top_tex, Texture* bottom_tex);
    ~SkyBox();

    void intersect(const Ray& r, Vector3f& color, Vector3f& normal);

protected:
    Rectangle top;
    Rectangle bottom;
    Rectangle left;
    Rectangle right;
    Rectangle front;
    Rectangle back;
    Vector3f center;

    Texture* front_tex;
    Texture* back_tex;
    Texture* right_tex;
    Texture* left_tex;
    Texture* top_tex;
    Texture* bottom_tex;
};


#endif