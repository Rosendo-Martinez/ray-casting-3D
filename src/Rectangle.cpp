#include "Rectangle.h"


Rectangle::Rectangle() {}


Rectangle::Rectangle(const Vector3f& normal, const Vector3f& center, const Vector3f& orientation, float width, float height, Material* m) 
    : Object3D(m), normal(normal), center(center), width(width), height(height)
{
    this->horizontal = orientation.normalized();
    this->vertical = Vector3f::cross(normal, this->horizontal).normalized();
    this->d = Vector3f::dot(center, normal.normalized());
}


Rectangle::~Rectangle() {}


bool Rectangle::intersect(const Ray& r, Hit& h, float tmin)
{
    // TODO: could just have a Plane() member for this class and call
    //       Plane().intersection() on it instead of doing plane intersection here (duplicate code).

    // Do ray-plane intersection

    float n_dot_rd = Vector3f::dot(normal, r.getDirection());

    // Ray and plane are parallel
    if (n_dot_rd == 0)
    {
        return false;
    }

    float n_dot_ro = Vector3f::dot(normal, r.getOrigin());
    float t = (d - n_dot_ro) / n_dot_rd;

    // Invalid t check
    if (t < tmin || t > h.getT())
    {
        return false;
    }

    // Do ray-square intersection

    Vector3f p = r.pointAtParameter(t);
    float horizontal_projected_distance = Vector3f::dot(p - center, horizontal);
    float vertical_projected_distance = Vector3f::dot(p - center, vertical);
    float half_width = width / 2.0f;
    float half_height = height / 2.0f;

    // Intersection point in square bounds check
    if 
    (
        horizontal_projected_distance < -half_width || 
        horizontal_projected_distance > half_width  ||
        vertical_projected_distance < -half_height  ||
        vertical_projected_distance > half_height
    )
    {
        return false;
    }

    // UV-mapping
    float u = (horizontal_projected_distance / width) + 0.5f;
    float v = (vertical_projected_distance / height) + 0.5f;
    assert(u >= 0.0f && u <= 1.0f);
    assert(v >= 0.0f && v <= 1.0f);

    Vector3f normal_at_hit = this->normal;

    // Bump perturbation
    if (this->material->bum.valid())
    {
        Vector2f gradient = this->material->bum.getGradient(u,v);
        normal_at_hit += (gradient.x() * horizontal) + (gradient.y() * vertical);
        normal_at_hit.normalize();
    }

    h.setTexCoord(Vector2f(u,v));
    h.set(t, material, normal_at_hit);
    return true;
}