#include "Sphere.h"


Sphere::Sphere()
	: center(Vector3f(0.0f)), radius(1.0f)
{}


// Sphere with default orientation of texture.
Sphere::Sphere(Vector3f center, float radius, Material* material)
	: Sphere(center, Vector3f(0,0,1), Vector3f(0,1,0), radius, material)
{
}


// The vectors left, and top edge determine orientation of texture on sphere.
Sphere::Sphere(Vector3f center, Vector3f left_edge, Vector3f top_edge, float radius, Material* material)
	: Object3D(material), radius(radius), center(center)
{
	assert(Vector3f::dot(left_edge, top_edge) == 0);

	// IDEA: maybe user can just pass in a 'UP' vector which I do gram-shmedit on to get other basis vectors?
	x = left_edge.normalized();
	z = top_edge.normalized();
	y = Vector3f::cross(z, x).normalized();
}


Sphere::~Sphere() {}


bool Sphere::intersect(const Ray& r, Hit& h, float tmin)
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
			Vector2f uv = get_uv(p);
			Vector3f norm = perturb_normal(normal_at_point(p), uv.x(), uv.y());

			h.set(t, material, norm);
			h.setTexCoord(uv);
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
		Vector2f uv = get_uv(p);
		Vector3f norm = perturb_normal(normal_at_point(p), uv.x(), uv.y());

		h.set(t_plus, material, norm);
		h.setTexCoord(uv);
		return true;
	}

	bool tMinus_legalT = t_minus >= tmin;
	bool tMinus_closerHit = t_minus < h.getT();

	if (!tPlus_closer_then_tMinus && tMinus_legalT && tMinus_closerHit)
	{
		Vector3f p = r.pointAtParameter(t_minus);
		Vector2f uv = get_uv(p);
		Vector3f norm = perturb_normal(normal_at_point(p), uv.x(), uv.y());

		h.set(t_minus, material, norm);
		h.setTexCoord(uv);
		return true;
	}
	
	return false;
}


Vector3f Sphere::normal_at_point(const Vector3f& p) const
{
	return (p - center) / radius;
}


// Only perturbs normal if has valid bump map.
Vector3f Sphere::perturb_normal(Vector3f normal_at_point, float u, float v) const
{
	if (material->bum.valid())
	{
		// NOTE: does not handle edge cases (YET).
		if (normal_at_point != z && normal_at_point != -z)
		{
			Vector3f u_basis = Vector3f::cross(z, normal_at_point).normalized();
			Vector3f v_basis = Vector3f::cross(normal_at_point, u_basis).normalized();
			Vector2f gradient = material->bum.getGradient(u,v);

			normal_at_point += gradient.x() * u_basis + gradient.y() * v_basis;
			normal_at_point.normalize();
		}
	}

	return normal_at_point;
}


Vector2f Sphere::get_uv(Vector3f point_on_sphere) const
{
	Vector3f norm = normal_at_point(point_on_sphere);

	// Spherical coordinates (SC)
	Vector3f p_sc = Matrix3f(x,y,z).inverse() * norm;
	float D = sqrt(p_sc.x() * p_sc.x() + p_sc.y() * p_sc.y());

	float angle_u = atan2(p_sc.y(), p_sc.x()); // [-pi, pi]
	float angle_v = atan2(D, p_sc.z()); // D >= 0 --> [0, pi]
	assert(angle_v >= 0);

	if (angle_u < 0)
	{
		assert(p_sc.y() < 0);
		angle_u += 2.0 * M_PI; // [0, 2pi]
	}

	float u = angle_u / (2.0 * M_PI); // [0, 2pi] --> [0,1]
	float v = 1 - (angle_v / M_PI);

	return Vector2f(u,v);
}