#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
#include <iostream>

using namespace std;
///TODO: implement this class.
///Add more fields as necessary,
///but do not remove hasTex, normals or texCoords
///they are filled in by other components
class Triangle : public Object3D
{
public:
	Triangle();

    ///@param a, b, c are three vertex positions of the triangle
	Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m)
		: Object3D(m), a(a), b(b), c(c)
	{
    	hasTex = false;
	}

	virtual bool intersect(const Ray& ray, Hit& hit, float tmin)
	{
		bool isHit = false;

		Matrix3f A;
		A.setCol(0, a - b);
		A.setCol(1, a - c);
		A.setCol(2, ray.getDirection());

		float A_det = A.determinant();

		float beta = Matrix3f(a - ray.getOrigin(), a - c, ray.getDirection()).determinant() / A_det;
		float gamma = Matrix3f(a - b, a - ray.getOrigin(), ray.getDirection()).determinant() / A_det;
		float alpha = 1.0f - beta - gamma;
		float t = Matrix3f(a - b, a - c, a - ray.getOrigin()).determinant() / A_det;

		bool isTriangleIntersection = (beta + gamma <= 1.0f) && (beta >= 0.0f) && (gamma >= 0.0f);
		bool isLegalT = (t >= tmin);
		bool isCloserIntersection = (t < hit.getT());

		if (isTriangleIntersection && isLegalT && isCloserIntersection)
		{
			Vector3f normal = ((alpha * normals[0]) + (beta * normals[1]) + (gamma * normals[2])).normalized();
			Vector2f tex = ((alpha * texCoords[0]) + (beta * texCoords[1]) + (gamma * texCoords[2])); // normalize???
			hit.set(t, this->material, normal);
			hit.setTexCoord(tex);
			isHit = true;
		}

		return isHit;
	}

	bool hasTex;
	Vector3f normals[3];
	Vector2f texCoords[3];

protected:
	Vector3f a;
	Vector3f b;
	Vector3f c;
};

#endif //TRIANGLE_H
