#include "Circle.h"


Circle::Circle() {}


Circle::Circle(const Vector3f& normal, const Vector3f& center, float radius, const Vector3f& tex_orin, Material* m) 
	: Object3D(m)
{
	this->normal = normal.normalized();
	this->radiusSquared = radius * radius;
	this->center = center;

	this->d = -Vector3f::dot(this->center, this->normal);
	this->tex_orin = tex_orin.normalized();
	this->z = Vector3f::cross(this->normal, this->tex_orin).normalized();
}


Circle::~Circle() {}


bool Circle::intersect(const Ray& r, Hit& h, float tmin)
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
	h.setTexCoord(getTexCoord(p));
	
	return true;
}


Vector2f Circle::getTexCoord(const Vector3f& p)
{
	constexpr float one_over_two_pi = (1.0f/(2.0f*M_PI));
	Vector3f p_minus_center = p - center;

	float projected_x = Vector3f::dot(p_minus_center, tex_orin);
	float projected_y = Vector3f::dot(p_minus_center, z);
	float angle = atan2(projected_y, projected_x); // [-pi, +pi]

	// Linear mappings from world to UV coordinates
	float u = one_over_two_pi * angle + 0.5f; // [0, 1]
	float v = p_minus_center.absSquared() / radiusSquared; // [0, 1]

	return Vector2f(u,v);
}