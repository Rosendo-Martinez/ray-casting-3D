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
	virtual ~Camera();

protected:
	Vector3f center; 
	Vector3f direction;
	Vector3f up;
	Vector3f horizontal;
};


class PerspectiveCamera: public Camera
{
public:
	PerspectiveCamera(const Vector3f& center, const Vector3f& direction, const Vector3f& up, float angle);

	virtual Ray generateRay(const Vector2f& point);
	virtual float getTMin() const;

private:
	float fov; // radians, horizontal
	Vector3f virtualScreenOffset;
};


class OrthographicCamera : public Camera
{
public:
	OrthographicCamera(const Vector3f& center, const Vector3f& direction, const Vector3f& up, float aspect, float scaling);

	virtual Ray generateRay(const Vector2f& point);
	virtual float getTMin() const;
};


#endif //CAMERA_H