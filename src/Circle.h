#ifndef CIRCLE_H
#define CIRCLE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>


class Circle : public Object3D
{
public:
	Circle() {}

	Circle(const Vector3f& normal, const Vector3f& center, float radius, Material* m) 
		: Object3D(m)
	{
        this->normal = normal.normalized();
        this->radiusSquared = radius * radius;
        this->center = center;

        this->d = -Vector3f::dot(this->center, this->normal);
    }

	~Circle() {}

	virtual bool intersect(const Ray& r, Hit& h, float tmin)
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

        Vector3f p = r.pointAtParameter(t);
        float distanceSquared = (p - center).absSquared();

        if (distanceSquared > radiusSquared)
        {
            return false;
        }

		h.set(t, material, normal);
		

		return true;
	}

protected:
	Vector3f normal = Vector3f(0.0f);
    Vector3f center = Vector3f(0.0f);
    float radiusSquared = 0.0f;
	float d = 0.0f;
};

#endif // CIRCLE_H
