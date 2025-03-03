#include "Triangle.h"


Triangle::Triangle() {}


/// @param a, b, c are three vertex positions of the triangle
Triangle::Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m)
	: Object3D(m), a(a), b(b), c(c)
{
	hasTex = false;
}


bool Triangle::intersect(const Ray& ray, Hit& hit, float tmin)
{
	const Vector3f ro = ray.getOrigin();
	const Vector3f rd = ray.getDirection();
	const Vector3f a_minus_ro = a - ro;
	const Vector3f a_minus_c = a - c;
	const Vector3f a_minus_b = a - b;

	Matrix3f A;
	A.setCol(0, a_minus_b);
	A.setCol(1, a_minus_c);
	A.setCol(2, rd);

	float A_det = A.determinant();

	float beta = Matrix3f(a_minus_ro, a_minus_c, rd).determinant() / A_det;
	float gamma = Matrix3f(a_minus_b, a_minus_ro, rd).determinant() / A_det;
	float alpha = 1.0f - beta - gamma;

	bool isIntersection = (beta + gamma <= 1.0f) && (beta >= 0.0f) && (gamma >= 0.0f);
	if (!isIntersection) // No intersection
	{
		return false;
	}

	float t = Matrix3f(a_minus_b, a_minus_c, a_minus_ro).determinant() / A_det;

	bool isLegalT = (t >= tmin);
	bool isCloserT = (t < hit.getT());
	if (!isLegalT || !isCloserT) 
	{
		return false;
	}

	// We have a hit
	Vector3f norm = ((alpha * normals[0]) + (beta * normals[1]) + (gamma * normals[2])).normalized();
	hit.set(t, material, norm, hasTex);
	if (hasTex)
	{
		Vector2f tex = ((alpha * texCoords[0]) + (beta * texCoords[1]) + (gamma * texCoords[2]));
		hit.setTexCoord(tex);
	}

	return true;
}