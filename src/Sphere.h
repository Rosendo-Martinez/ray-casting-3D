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
	Sphere()
		: center(Vector3f(0.0f)), radius(1.0f)
	{}

	Sphere(Vector3f center, Vector3f left_edge, Vector3f top_edge, float radius, Material* material)
		: Object3D(material), radius(radius), center(center)
	{
		assert(Vector3f::dot(left_edge, top_edge) == 0);

		// IDEA: maybe user can just pass in a 'UP' vector which I do gram-shmidit on to get other basis vectors?
		x = left_edge.normalized();
		z = top_edge.normalized();
		y = Vector3f::cross(z, x).normalized();
	}
	
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
		float discriminant = Quadratic::discriminant(a, b, c);

		// Case: no intersection

		if (discriminant < 0)
		{
			return false;
		}

		// Case: intersection at 1 point

		if (discriminant == 0)
		{
			float t = Quadratic::root(a, b);
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

		float discriminant_sqrt = std::sqrt(discriminant);
		float t_plus = Quadratic::rootRight(a, b, discriminant_sqrt);
		float t_minus = Quadratic::rootLeft(a, b, discriminant_sqrt);

		bool tPlus_closer_then_tMinus = t_plus < t_minus;
		bool tPlus_legalT = t_plus >= tmin;
		bool tPlus_closerHit = t_plus < h.getT();

		if (tPlus_closer_then_tMinus && tPlus_legalT && tPlus_closerHit)
		{
			Vector3f p = r.pointAtParameter(t_plus);
			Vector3f norm = normalAtPoint(p);

			// Spherical coordinates (SC)
			Vector3f p_sc = Matrix3f(x, y, z) * norm;
			float D = sqrt(p_sc.x() * p_sc.x() + p_sc.y() * p_sc.y());

			float angle_u = atan2(p_sc.y(), p_sc.x()); // [-pi, pi]
			float angle_v = atan2(D, p_sc.z()); // D >= 0 --> [0, pi]

			if (angle_u < 0)
			{
				angle_u += 2.0 * M_PI; // [0, 2pi]
			}

			float u = angle_u / (2.0 * M_PI);
			float v = 1 - (angle_v / M_PI);

			h.set(t_plus, material, norm);
			h.setTexCoord(Vector2f(u,v));
			return true;
		}

		bool tMinus_legalT = t_minus >= tmin;
		bool tMinus_closerHit = t_minus < h.getT();

		if (!tPlus_closer_then_tMinus && tMinus_legalT && tMinus_closerHit)
		{
			Vector3f p = r.pointAtParameter(t_minus);
			Vector3f norm = normalAtPoint(p);

			// Spherical coordinates (SC)
			Vector3f p_sc = Matrix3f(x, y, z) * norm;
			float D = sqrt(p_sc.x() * p_sc.x() + p_sc.y() * p_sc.y());

			float angle_u = atan2(p_sc.y(), p_sc.x()); // [-pi, pi]
			float angle_v = atan2(D, p_sc.z()); // D >= 0 --> [0, pi]

			if (angle_u < 0)
			{
				angle_u += 2.0 * M_PI; // [0, 2pi]
			}

			float u = angle_u / (2.0 * M_PI);
			float v = 1 - (angle_v / M_PI);

			h.set(t_minus, material, norm);
			h.setTexCoord(Vector2f(u,v));
			return true;
		}
		
		return false;
	}

protected:
	const Vector3f center;
	const float radius;
	Vector3f x;
	Vector3f y;
	Vector3f z;

private:
	Vector3f normalAtPoint(const Vector3f& p) const
	{
		return (p - center) / radius;
	}
	
};


#endif