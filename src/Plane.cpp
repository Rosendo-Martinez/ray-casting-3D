#include "Plane.h"


Plane::Plane() {}


Plane::Plane(const Vector3f& normal, float d, Material* m) 
	: Object3D(m), normal(normal), d(d)
{}


Plane::~Plane() {}


bool Plane::intersect(const Ray& r, Hit& h, float tmin)
{
	float n_dot_rd = Vector3f::dot(normal, r.getDirection());

	// Ray and plane are parallel
	if (n_dot_rd == 0)
	{
		return false;
	}

	float n_dot_ro = Vector3f::dot(normal, r.getOrigin());
	float t = (d - n_dot_ro) / n_dot_rd;

	if (t < tmin || t > h.getT())
	{
		return false;
	}

	h.set(t, material, normal);
	return true;
}