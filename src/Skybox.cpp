#include "Skybox.h"


// Square --------------------------------------------------------------------------------------------------------


Square::Square() {}


Square::Square(const Vector3f& normal, const Vector3f& center, const Vector3f& orientation, float width, float height, Material* m) 
    : Object3D(m), normal(normal), center(center), width(width), height(height)
{
    this->horizontal = orientation.normalized();
    this->vertical = Vector3f::cross(normal, this->horizontal).normalized();
    this->d = Vector3f::dot(center, normal.normalized());
}


Square::~Square() {}


bool Square::intersect(const Ray& r, Hit& h, float tmin)
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


// SkyBox -------------------------------------------------------


SkyBox::SkyBox() {}


SkyBox::SkyBox(const Vector3f& center, float angle, Texture* front_tex, Texture* back_tex, Texture* right_tex, Texture* left_tex, Texture* top_tex, Texture* bottom_tex)
    : center(center), front_tex(front_tex), back_tex(back_tex), right_tex(right_tex), left_tex(left_tex), top_tex(top_tex), bottom_tex(bottom_tex)
{
    // Initialize faces of skycube

    Vector3f c_front  (0,0,-1);
    Vector3f c_back   (0,0,1);
    Vector3f c_right  (1,0,0);
    Vector3f c_left   (-1,0,0);
    Vector3f c_top    (0,1,0);
    Vector3f c_bottom (0,-1,0);

    Vector3f n_front  (0,0,1);
    Vector3f n_back   (0,0,-1);
    Vector3f n_right  (-1,0,0);
    Vector3f n_left   (1,0,0);
    Vector3f n_top    (0,-1,0);
    Vector3f n_bottom (0,1,0);

    Vector3f orin_front  (1,0,0);
    Vector3f orin_back   (-1,0,0);
    Vector3f orin_right  (0,0,1);
    Vector3f orin_left   (0,0,-1);
    Vector3f orin_top    (1,0,0);
    Vector3f orin_bottom (1,0,0);


    // Rotates skycube about +y-axis at given angle.
    Matrix3f rotate (
        Vector3f(cos(angle), 0, -sin(angle)),
        Vector3f(0,1,0),
        Vector3f(sin(angle), 0, cos(angle))
    );

    // Big lengths can cause floating point errors!
    float length = 1;

    // Square does checks on material.
    Material* dummy_mat = new Material(Vector3f(1.0f));

    front =  Square(rotate * n_front,  (length/2.0f) * (rotate * c_front) + center,  rotate * orin_front,  length, length, dummy_mat);
    back =   Square(rotate * n_back,   (length/2.0f) * (rotate * c_back) + center,   rotate * orin_back,   length, length, dummy_mat);
    right =  Square(rotate * n_right,  (length/2.0f) * (rotate * c_right) + center,  rotate * orin_right,  length, length, dummy_mat);
    left =   Square(rotate * n_left,   (length/2.0f) * (rotate * c_left) + center,   rotate * orin_left,   length, length, dummy_mat);
    top =    Square(rotate * n_top,    (length/2.0f) * (rotate * c_top) + center,    rotate * orin_top,    length, length, dummy_mat);
    bottom = Square(rotate * n_bottom, (length/2.0f) * (rotate * c_bottom) + center, rotate * orin_bottom, length, length, dummy_mat);
}


SkyBox::~SkyBox() {}


void SkyBox::intersect(const Ray& r, Vector3f& color, Vector3f& normal)
{
    Hit h;
    float tmin = 0.0f;

    if (front.intersect(r,h,tmin))
    {
        normal = h.getNormal();
        color = front_tex->operator()(h.texCoord.x(), h.texCoord.y());
        return;
    }
    else if (back.intersect(r,h,tmin))
    {
        normal = h.getNormal();
        color = back_tex->operator()(h.texCoord.x(), h.texCoord.y());
        return;
    }
    else if (right.intersect(r,h,tmin))
    {
        normal = h.getNormal();
        color = right_tex->operator()(h.texCoord.x(), h.texCoord.y());
        return;
    }
    else if (left.intersect(r,h,tmin))
    {
        normal = h.getNormal();
        color = left_tex->operator()(h.texCoord.x(), h.texCoord.y());
        return;
    }
    else if (top.intersect(r,h,tmin))
    {
        normal = h.getNormal();
        color = top_tex->operator()(h.texCoord.x(), h.texCoord.y());
        return;
    }
    else if (bottom.intersect(r,h,tmin))
    {
        normal = h.getNormal();
        color = bottom_tex->operator()(h.texCoord.x(), h.texCoord.y());
        return;
    }

    // Error: should have intersected some face of the skycube!
    assert(false);

    return;
}