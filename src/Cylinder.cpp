#include "Cylinder.h"


Cylinder::Cylinder() {}


/**
 * tex_orin is used to orientate the texture on the cylinder.
 */
Cylinder::Cylinder(const Vector3f& e1, const Vector3f& e2, float radius, const Vector3f& tex_orin, Material* m)
    : Object3D(m), endpoint_1(e1), endpoint_2(e2), radius(radius)
{
    this->tex_orin = tex_orin.normalized();
    this->normal = (endpoint_2 - endpoint_1).normalized();
    this->z = Vector3f::cross(this->normal, this->tex_orin).normalized();
    this->height = (endpoint_2 - endpoint_1).abs();
}


bool Cylinder::intersect(const Ray& r, Hit& hit, float tmin)
{
    Vector3f rd = r.getDirection();
    Vector3f ro = r.getOrigin();

    float rd_dot_n = Vector3f::dot(rd, normal);
    float ro_dot_n = Vector3f::dot(ro, normal);
    float c_dot_n = Vector3f::dot(endpoint_1, normal);

    Vector3f A = (rd_dot_n * normal) - rd;
    Vector3f B = (ro_dot_n * normal) - (c_dot_n * normal) - ro + endpoint_1;

    float a = Vector3f::dot(A, A);
    float b = 2 * Vector3f::dot(A, B);
    float c = Vector3f::dot(B, B) - (radius * radius);

    float discriminant = Quadratic::discriminant(a, b, c);

    // Zero hits
    if (discriminant < 0)
    {
        return false;
    }

    // One hit
    if (discriminant == 0)
    {
        float t = Quadratic::root(a, b);

        if (t < tmin || t > hit.getT())
        {
            return false;
        }

        return handle_potential_hit_on_finite_cylinder(r, hit, t);
    }

    // Two hits

    float discriminant_sqrt = std::sqrt(discriminant);
    float t_plus = Quadratic::rootRight(a, b, discriminant_sqrt);
    float t_minus = Quadratic::rootLeft(a, b, discriminant_sqrt);

    if (t_plus < t_minus && t_plus >= tmin && t_plus < hit.getT())
    {
        return handle_potential_hit_on_finite_cylinder(r, hit, t_plus);
    }

    if (t_minus < t_plus && t_minus >= tmin && t_minus < hit.getT())
    {
        return handle_potential_hit_on_finite_cylinder(r, hit, t_minus);
    }

    return false;
}


Vector2f Cylinder::getTexCoord(const Vector3f& p) const
{
    constexpr float one_over_two_pi = (1.0f/(2.0f*M_PI));
    Vector3f p_minus_e1 = p - endpoint_1;

    float projected_height = Vector3f::dot(p_minus_e1, normal);
    float projected_x = Vector3f::dot(p_minus_e1, tex_orin);
    float projected_y = Vector3f::dot(p_minus_e1, z);
    float angle = atan2(projected_y, projected_x); // [-pi, +pi]

    // Linear mappings from world to UV coordinates
    float u = one_over_two_pi * angle + 0.5f; // [0, 1]
    float v = projected_height / height; // [0, 1]

    return Vector2f(u,v);
}


// Returns perturbed normal if has bump map, else does nothing.
Vector3f Cylinder::perturb_normal(Vector3f normal_at_point, float u, float v) const
{
    if (material->bum.valid())
    {
        Vector2f gradient = material->bum.getGradient(u,v);
        Vector3f u_basis = Vector3f::cross(normal, normal_at_point);
        Vector3f v_basis = normal;

        normal_at_point += gradient.x() * u_basis + gradient.y() * v_basis;
        normal_at_point.normalize();
    }

    return normal_at_point;
}


// ASSUMPTION: ray hit infinite cylinder.
bool Cylinder::handle_potential_hit_on_finite_cylinder(const Ray& r, Hit& hit, float t) const
{
    Vector3f p = r.pointAtParameter(t);
    float projected_height = Vector3f::dot(p - endpoint_1, normal);

    // Check if only hit infinite cylinder.
    if (projected_height < 0 || projected_height > height)
    {
        return false;
    }
    
    Vector2f uv = getTexCoord(p);
    Vector3f normal_at_hit = (p - (projected_height * normal + endpoint_1)) / radius;
    normal_at_hit = perturb_normal(normal_at_hit, uv.x(), uv.y());

    hit.set(t, material, normal_at_hit);
    hit.setTexCoord(uv);
    return true;
}