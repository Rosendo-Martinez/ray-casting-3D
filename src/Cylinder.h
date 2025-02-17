#ifndef CYLINDER_H
#define CYLINDER_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

class Cylinder : public Object3D
{
public:
    Cylinder() {}

    Cylinder(const Vector3f& e1, const Vector3f& e2, float radius, Material* m)
        : Object3D(m), endpoint_1(e1), endpoint_2(e2), radius(radius)
    {
        normal = (endpoint_2 - endpoint_1).normalized();

        std::cout << "E1: "; e1.print();
        std::cout << "E2: "; e2.print();
        std::cout << "R: " << radius << '\n';
    }

    virtual bool intersect(const Ray& r, Hit& hit, float tmin)
    {
        // std::cout << "Cylinder::intersect()\n";

        Vector3f rd = r.getDirection();
        Vector3f ro = r.getOrigin();
        Vector3f center = endpoint_1;

        float rd_dot_n = Vector3f::dot(rd, normal);
        float ro_dot_n = Vector3f::dot(ro, normal);
        float c_dot_n = Vector3f::dot(center, normal);

        Vector3f A = (rd_dot_n * normal) - rd;
        Vector3f B = (ro_dot_n * normal) - (c_dot_n * normal) - ro + center;

        float a = Vector3f::dot(A, A);
        float b = 2 * Vector3f::dot(A, B);
        float c = Vector3f::dot(B, B) - (radius * radius);

        float d_sqr = (b * b) - (4 * a * c);

        // No hit
        if (d_sqr < 0)
        {
            // std::cout << "no hit 1\n";
            return false;
        }

        // One hit
        if (d_sqr == 0)
        {
            float t = -b / (2 * a);

            if (t < tmin || t > hit.getT())
            {
                std::cout << "no hit 2\n";
                return false;
            }

            Vector3f p = r.pointAtParameter(t);
            Vector3f p_minus_center = p - center;
            float DISTANCE = Vector3f::dot(p_minus_center, normal);
            Vector3f normal_at_hit = DISTANCE * normal - p_minus_center;
            normal_at_hit.normalize();

            std::cout << "one hit\n";
            hit.set(t, material, normal_at_hit);
            return true;
        }

        // Two hits

        float d = std::sqrt(d_sqr);
        float t_plus = (-b + d) / (2 * a);
        float t_minus = (-b - d) / (2 * a);

        if (t_plus < t_minus && t_plus >= tmin && t_plus < hit.getT())
        {
            std::cout << "t_plus hit\n";
            Vector3f p = r.pointAtParameter(t_plus);
            Vector3f p_minus_center = p - center;
            float DISTANCE = Vector3f::dot(p_minus_center, normal);
            Vector3f normal_at_hit = DISTANCE * normal - p_minus_center;
            normal_at_hit.normalize();

            hit.set(t_plus, material, normal_at_hit);
            return true;
        }

        if (t_minus < t_plus && t_minus >= tmin && t_minus < hit.getT())
        {
            std::cout << "t_minus hit\n";
            Vector3f p = r.pointAtParameter(t_minus);
            Vector3f p_minus_center = p - center;
            float DISTANCE = Vector3f::dot(p_minus_center, normal);
            Vector3f normal_at_hit = DISTANCE * normal - p_minus_center;
            normal_at_hit.normalize();

            hit.set(t_minus, material, normal_at_hit);
            return true;
        }

        std::cout << "no hit 3\n";
        return false;
    }

private:
    Vector3f endpoint_1;
    Vector3f endpoint_2;
    Vector3f normal;
    float radius;
};

#endif