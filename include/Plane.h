#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>


class Plane : public Object3D
{
public:
	Plane();
	Plane(const Vector3f& normal, float d, Material* m);
	~Plane();

	virtual bool intersect(const Ray& r, Hit& h, float tmin);

protected:
	const Vector3f normal = Vector3f(0.0f);
	const float d = 0.0f;
};


#endif // PLANE_H