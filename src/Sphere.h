#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

#include <iostream>
using namespace std;
///TODO:
///Implement functions and add more fields as necessary
class Sphere: public Object3D
{
public:
	Sphere(){ 
		//unit ball at the center
		center = Vector3f(0.f);
		radius = 1.0f;
	}

	Sphere( Vector3f center , float radius , Material* material ):Object3D(material){
		this->center = center;
		this->radius = radius;

		std::cout << "Sphere:\n";
		std::cout << "    center: "; this->center.print();
		std::cout << "    radius: " << this->radius << '\n';
		std::cout << "    color: "; this->material->getDiffuseColor().print();
	}
	

	~Sphere(){}

	virtual bool intersect( const Ray& r , Hit& h , float tmin){
		// float a = Vector3f::dot(r.getDirection(), r.getDirection());
		float a = 1; // assumes R_d is normalized
		float b = 2 * Vector3f::dot(r.getDirection(), r.getOrigin());
		float c = Vector3f::dot(r.getOrigin(),r.getOrigin()) - (this->radius * this->radius);
		float discriminantSqr = (b*b) - (4*a*c);

		// std::cout << "h.t = " << h.getT() << '\n';
		if (discriminantSqr < 0) // no hit
		{
			// std::cout << "No hit\n";
			return false;
		}
		else if (discriminantSqr == 0) // one hit
		{
			// std::cout << "One hit\n";
			float t = (-b) / (2 * a);
			if (t >= tmin && t < h.getT() && t >= 0)
			{
				Vector3f normal = (r.pointAtParameter(t) - r.getOrigin()) / this->radius;
				h.set(t, this->material, normal);
				return true;
			}
		}
		else // two hits
		{
			float d = std::sqrt(discriminantSqr);
			float t_plus = (-b + d) / (2 * a);
			float t_minus = (-b - d) / (2 * a);
			// std::cout << "T_+: " << t_plus << '\n';
			// std::cout << "T_-: " << t_minus << '\n';

			if (t_plus < t_minus && t_plus >= 0 && t_plus >= tmin && t_plus < h.getT()) // t_plus is closer and positive
			{
				Vector3f normal = (r.pointAtParameter(t_plus) - r.getOrigin()) / this->radius;
				h.set(t_plus, this->material, normal);
				return true;
			}
			else if (t_minus < t_plus && t_minus >= 0 && t_minus >= tmin && t_minus < h.getT()) // t_minus is closer and positive
			{
				Vector3f normal = (r.pointAtParameter(t_minus) - r.getOrigin()) / this->radius;
				h.set(t_minus, this->material, normal);
				return true;
			}
		}
		
		return false;
	}

protected:
	Vector3f center;
	float radius;
};


#endif
