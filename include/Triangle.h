#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>


class Triangle : public Object3D
{
public:
	Triangle();
    /// @param a, b, c are three vertex positions of the triangle
	Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m);

	virtual bool intersect(const Ray& ray, Hit& hit, float tmin);

	// Filled in by other components
	bool hasTex;
	Vector3f normals[3];
	Vector2f texCoords[3];

protected:
	Vector3f a;
	Vector3f b;
	Vector3f c;
};


#endif // TRIANGLE_H