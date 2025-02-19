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

class OrthographicCamera : public Camera
{
public:
	OrthographicCamera(Vector3f center, Vector3f direction, Vector3f up, float aspect, float scaling)
	{
		this->center = center;
		this->direction = direction.normalized();

		this->horizontal = Vector3f::cross(this->direction, up).normalized();
		this->up = Vector3f::cross(this->horizontal, this->direction).normalized();

		this->aspect = aspect;
		this->scaling = scaling;

		std::cout << "Hello From Ortho Cam!\n";
		std::cout << "center: "; this->center.print();
		std::cout << "dir: "; this->direction.print();
		std::cout << "hor: "; this->horizontal.print();
		std::cout << "up: "; this->up.print();
		std::cout << "aspect: " << this->aspect << '\n';
		std::cout << "scaling: " << this->scaling << '\n';
	};

	virtual Ray generateRay(const Vector2f& point)
	{
		// vs = virtual screen
		float height_vs = scaling;
		float width_vs = aspect * scaling;

		// Map from image to virtual image.
		float x_vs = (width_vs / 2.0f) * point.x();
		float y_vs = (height_vs / 2.0f) * point.y();

		Vector3f ray_base = (y_vs * up) + (x_vs * horizontal) + center;
		ray_base.print();
		
		Ray ray (ray_base, direction);

		return ray;
	}

	virtual float getTMin() const
	{
		return 0.0f;
	}

private:
	float aspect; // width / height
	float scaling;
};

#endif //CAMERA_H
