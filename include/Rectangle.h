#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Object3D.h"
#include <cassert>


class Rectangle : public Object3D
{   
public:
	Rectangle();
	Rectangle(const Vector3f& normal, const Vector3f& center, const Vector3f& orientation, float width, float height, Material* m);
	~Rectangle();

	virtual bool intersect(const Ray& r, Hit& h, float tmin);

protected:
	Vector3f normal;
    Vector3f center;
    Vector3f horizontal; // alines with texture u
    Vector3f vertical;   // alines with texture v
    float width;
    float height;
	float d;             // projected distance from center to origin
};


#endif