#ifndef CYLINDER_H
#define CYLINDER_H

#include "Object3D.h"
#include "QuadraticFormula.h"
#include <vecmath.h>
#include <cmath>

class Cylinder : public Object3D
{
public:
    Cylinder();

    /**
     * tex_orin is used to orientate the texture on the cylinder.
     */
    Cylinder(const Vector3f& e1, const Vector3f& e2, float radius, const Vector3f& tex_orin, Material* m);

    virtual bool intersect(const Ray& r, Hit& hit, float tmin);

private:
    Vector3f endpoint_1; // origin of cylinder
    Vector3f endpoint_2;
    Vector3f normal;
    float radius;
    float height;
    Vector3f tex_orin;
    Vector3f z;

    Vector2f getTexCoord(const Vector3f& p) const;
    Vector3f perturb_normal(Vector3f normal_at_point, float u, float v) const;
    bool handle_potential_hit_on_finite_cylinder(const Ray& r, Hit& hit, float t) const;
};

#endif