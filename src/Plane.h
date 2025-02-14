#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
using namespace std;


class Plane : public Object3D
{
public:
	Plane() {}

	Plane(const Vector3f& normal, float d, Material* m) 
		: Object3D(m), normal(normal), d(d)
	{}

	~Plane() {}

	virtual bool intersect(const Ray& r, Hit& h, float tmin)
	{
		float n_dot_rd = Vector3f::dot(this->normal, r.getDirection());

		bool isRayDirectionParallelToPlan = (n_dot_rd == 0);
		if (isRayDirectionParallelToPlan) // many or zero intersections
		{
			return false;
		}

		float t = -(this->d + Vector3f::dot(this->normal, r.getOrigin())) / n_dot_rd;
		
		bool isLegalIntersection = (t >= 0.0f && t >= tmin);
		bool isCloserIntersection = (t < h.getT());
		if (isLegalIntersection && isCloserIntersection)
		{
			// In some cases normal may need to be negative! 
			h.set(t, this->material, this->normal);
			return true;
		}

		return false;
	}

protected:
	const Vector3f normal = Vector3f(0.0f);
	const float d = 0.0f;
};

#endif //PLANE_H
