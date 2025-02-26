#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include "QuadraticFormula.h"
#include <vecmath.h>
#include <cmath>
#include <iostream>


class Sphere : public Object3D
{
public:
	Sphere();
	Sphere(Vector3f center, float radius, Material* material);
	Sphere(Vector3f center, Vector3f left_edge, Vector3f top_edge, float radius, Material* material);
	~Sphere();

	virtual bool intersect(const Ray& r, Hit& h, float tmin);

protected:
	const Vector3f center;
	const float radius;
	Vector3f x;
	Vector3f y;
	Vector3f z;

private:
	Vector3f normalAtPoint(const Vector3f& p) const;
};


#endif