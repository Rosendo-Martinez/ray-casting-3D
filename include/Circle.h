#ifndef CIRCLE_H
#define CIRCLE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>


class Circle : public Object3D
{
public:
	Circle();
	Circle(const Vector3f& normal, const Vector3f& center, float radius, const Vector3f& tex_orin, Material* m);

	~Circle();

	virtual bool intersect(const Ray& r, Hit& h, float tmin);

protected:
	Vector3f normal = Vector3f(0.0f);
    Vector3f center = Vector3f(0.0f);
	Vector3f tex_orin;
	Vector3f z;
    float radiusSquared = 0.0f;
	float d = 0.0f;

	Vector2f getTexCoord(const Vector3f& p);
};

#endif // CIRCLE_H