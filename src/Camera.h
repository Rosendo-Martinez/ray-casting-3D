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
	virtual Ray generateRay( const Vector2f& point ) = 0 ; 
	
	virtual float getTMin() const = 0 ; 
	virtual ~Camera(){}
protected:
	Vector3f center; 
	Vector3f direction;
	Vector3f up;
	Vector3f horizontal;

};

///TODO: Implement Perspective camera
///Fill in functions and add more fields if necessary
class PerspectiveCamera: public Camera
{
public:
	PerspectiveCamera(const Vector3f& center, const Vector3f& direction,const Vector3f& up , float angle){
		this->center = center;
		this->fov = angle;

		// Camera basis vectors
		this->direction = direction;
		this->horizontal = Vector3f::cross(this->direction, up).normalized();
		this->up = Vector3f::cross(this->horizontal, this->direction).normalized();

		// Virtual screen
		this->screenOffset = direction * (1.0f / tan(this->fov/2.0f));

		std::cout << "Angle: " << angle << '\n';
		std::cout << "Camera Center: ";
		this->center.print();
		std::cout << "Camera Direction: ";
		this->direction.print();
		std::cout << "Camera Horizontal: ";
		this->horizontal.print();
		std::cout << "Camera Up: ";
		this->up.print();
		std::cout << "Screen Offset: ";
		this->screenOffset.print();
	}

	virtual Ray generateRay( const Vector2f& point){
		// ASSUMPTION: No aspect ratio? I assume image will be square, for now.
		Vector3f pointOnScreen = (point.x() * this->horizontal) + (point.y() * this->up) + screenOffset + this->center;
		Vector3f rayDir = (pointOnScreen - this->center).normalized();
		Ray ray = Ray(this->center, rayDir);

		// std::cout << "Point: "; point.print();
		// std::cout << "Point On Screen: "; pointOnScreen.print();
		// std::cout << "Ray Dir: "; ray.getDirection().print();
		// std::cout << "Ray Org: "; ray.getOrigin().print();

		return ray;
	}

	virtual float getTMin() const { 
		return 0.0f;
	}

private:
	float fov; // horizontal
	Vector3f screenOffset;

};

#endif //CAMERA_H
