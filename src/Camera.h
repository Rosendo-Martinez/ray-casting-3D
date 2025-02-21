#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include <vecmath.h>
#include <float.h>
#include <cmath>
#include <cassert>

class Camera
{
public:
	//generate rays for each screen-space coordinate
	virtual Ray generateRay(const Vector2f& point) = 0; 
	
	virtual float getTMin() const = 0; 
	virtual ~Camera() {}

protected:
	Vector3f center; 
	Vector3f direction;
	Vector3f up;
	Vector3f horizontal;
};

class PerspectiveCamera: public Camera
{
public:
	PerspectiveCamera(const Vector3f& center, const Vector3f& direction, const Vector3f& up, float angle)
	{
		// MY ASSUMPTION!
		assert((direction.normalized() != up.normalized()) && (direction.normalized() != -up.normalized()));

		this->center = center; // camera position
		this->fov = angle; // radians, horizontal
		this->direction = direction.normalized(); // looking at

		// Calculate camera basis vectors
		this->horizontal = Vector3f::cross(this->direction, up).normalized();
		this->up = Vector3f::cross(this->horizontal, this->direction).normalized();

		// Calculate virtual screen offset
		this->virtualScreenOffset = this->direction * (1.0f / tan(this->fov/2.0f));
	}

	// ASSUMPTION: Only handles square images, for now.
	virtual Ray generateRay(const Vector2f& point)
	{
		Vector3f pointOnScreen = (point.x() * this->horizontal) + (point.y() * this->up) + virtualScreenOffset + this->center;
		Vector3f rayDir = (pointOnScreen - this->center).normalized();

		return Ray(this->center, rayDir);
	}

	virtual float getTMin() const
	{ 
		return 0.0f;
	}

private:
	float fov; // radians, horizontal
	Vector3f virtualScreenOffset;
};

class OrthographicCamera : public Camera
{
public:
	OrthographicCamera(const Vector3f& center, const Vector3f& direction, const Vector3f& up, float aspect, float scaling)
	{
		this->center = center;
		this->direction = direction.normalized();

		float half_height_vs = scaling / 2.0f;
		float half_width_vs = aspect * scaling / 2.0f;

		this->horizontal = Vector3f::cross(this->direction, up).normalized() * half_width_vs;
		this->up = Vector3f::cross(this->horizontal, this->direction).normalized() * half_height_vs;
	};

	virtual Ray generateRay(const Vector2f& point)
	{
		return Ray((point.x() * horizontal) + (point.y() * up) + center, direction);
	}

	virtual float getTMin() const
	{
		return 0.0f;
	}
};

#endif //CAMERA_H
