#include "Camera.h"


// Camera ----------------------------------------

Camera::~Camera() {}


// PerspectiveCamera ----------------------------

PerspectiveCamera::PerspectiveCamera(const Vector3f& center, const Vector3f& direction, const Vector3f& up, float angle)
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
Ray PerspectiveCamera::generateRay(const Vector2f& point)
{
	Vector3f pointOnScreen = (point.x() * this->horizontal) + (point.y() * this->up) + virtualScreenOffset + this->center;
	Vector3f rayDir = (pointOnScreen - this->center).normalized();

	return Ray(this->center, rayDir);
}

float PerspectiveCamera::getTMin() const
{ 
	return 0.0f;
}


// OrthographicCamera ---------------------------

OrthographicCamera::OrthographicCamera(const Vector3f& center, const Vector3f& direction, const Vector3f& up, float aspect, float scaling)
{
	this->center = center;
	this->direction = direction.normalized();

	float half_height_vs = scaling / 2.0f;
	float half_width_vs = aspect * scaling / 2.0f;

	this->horizontal = Vector3f::cross(this->direction, up).normalized() * half_width_vs;
	this->up = Vector3f::cross(this->horizontal, this->direction).normalized() * half_height_vs;
}

Ray OrthographicCamera::generateRay(const Vector2f& point)
{
	return Ray((point.x() * horizontal) + (point.y() * up) + center, direction);
}

float OrthographicCamera::getTMin() const
{
	return 0.0f;
}