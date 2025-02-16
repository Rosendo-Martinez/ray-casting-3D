#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include <vecmath.h>
#include <float.h>
#include <cmath>


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

#endif //CAMERA_H
