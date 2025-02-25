#ifndef SKYBOX_H
#define SKYBOX_H

#include "Object3D.h"
#include <cassert>


class Square : public Object3D
{   
public:
	Square();
	Square(const Vector3f& normal, const Vector3f& center, const Vector3f& orientation, float width, float height, Material* m);
	~Square();

	virtual bool intersect(const Ray& r, Hit& h, float tmin);

protected:
	Vector3f normal;     // surface normal
    Vector3f center;     // center of square
    Vector3f horizontal; // alines with texture u
    Vector3f vertical;   // alines with texture v
    float width;         // square width
    float height;        // square height
	float d;             // projected distance from center to origin
};


class SkyBox
{
public:
    SkyBox();
    SkyBox(const Vector3f& center, float angle, Texture* front_tex, Texture* back_tex, Texture* right_tex, Texture* left_tex, Texture* top_tex, Texture* bottom_tex);
    ~SkyBox();

    void intersect(const Ray& r, Vector3f& color, Vector3f& normal);

protected:
    Square top;
    Square bottom;
    Square left;
    Square right;
    Square front;
    Square back;
    Vector3f center;

    Texture* front_tex;
    Texture* back_tex;
    Texture* right_tex;
    Texture* left_tex;
    Texture* top_tex;
    Texture* bottom_tex;
};


#endif