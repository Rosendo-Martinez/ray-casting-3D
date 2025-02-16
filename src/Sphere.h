#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
#include <iostream>


class Sphere : public Object3D
{
public:
	Sphere()
		: center(Vector3f(0.0f)), radius(1.0f)
	{}

	Sphere(Vector3f center, float radius, Material* material)
		: Object3D(material), radius(radius), center(center)
	{}
	
	~Sphere() {}

	virtual bool intersect(const Ray& r, Hit& h, float tmin)
	{
		const float rd_dot_rd = Vector3f::dot(r.getDirection(), r.getDirection());
		const float ro_dot_ro = Vector3f::dot(r.getOrigin(), r.getOrigin());
		const float rd_dot_ro = Vector3f::dot(r.getDirection(), r.getOrigin());
		const float rd_dot_c = Vector3f::dot(r.getDirection(), center);
		const float ro_dot_c = Vector3f::dot(r.getOrigin(), center);
		const float c_dot_c = Vector3f::dot(center, center);

		float a = rd_dot_rd;
		float b = 2 * (rd_dot_ro - rd_dot_c);
		float c = ro_dot_ro + (-2.0f * ro_dot_c) + c_dot_c - (radius * radius);
		float d_squared = (b * b) - (4 * a * c);

		// Case: no intersection

		if (d_squared < 0)
		{
			return false;
		}

		// Case: intersection at 1 point

		if (d_squared == 0)
		{
			float t = (-b) / (2 * a);
			bool legalT = t >= tmin;
			bool closerHit = t < h.getT();

			if (legalT && closerHit)
			{
				Vector3f p = r.pointAtParameter(t);
				h.set(t, material, normalAtPoint(p));

				return true;
			}

			return false;
		}

		// Case: intersection at 2 points

		float d = std::sqrt(d_squared);
		float t_plus = (-b + d) / (2 * a);
		float t_minus = (-b - d) / (2 * a);

		bool tPlus_closer_then_tMinus = t_plus < t_minus;
		bool tPlus_legalT = t_plus >= tmin;
		bool tPlus_closerHit = t_plus < h.getT();

		if (tPlus_closer_then_tMinus && tPlus_legalT && tPlus_closerHit)
		{
			Vector3f p = r.pointAtParameter(t_plus);
			h.set(t_plus, material, normalAtPoint(p));

			return true;
		}

		bool tMinus_legalT = t_minus >= tmin;
		bool tMinus_closerHit = t_minus < h.getT();

		if (!tPlus_closer_then_tMinus && tMinus_legalT && tMinus_closerHit)
		{
			Vector3f p = r.pointAtParameter(t_minus);
			h.set(t_minus, material, normalAtPoint(p));

			return true;
		}
		
		return false;
	}

protected:
	const Vector3f center;
	const float radius;

private:
	Vector3f normalAtPoint(const Vector3f& p) const
	{
		return (p - center) / radius;
	}
	
};


#endif