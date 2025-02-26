#include "Skybox.h"


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

    // Rectangle does checks on material. Hack solution:
    Material* dummy_mat = new Material(Vector3f(1.0f));

    front =  Rectangle(rotate * n_front,  (length/2.0f) * (rotate * c_front) + center,  rotate * orin_front,  length, length, dummy_mat);
    back =   Rectangle(rotate * n_back,   (length/2.0f) * (rotate * c_back) + center,   rotate * orin_back,   length, length, dummy_mat);
    right =  Rectangle(rotate * n_right,  (length/2.0f) * (rotate * c_right) + center,  rotate * orin_right,  length, length, dummy_mat);
    left =   Rectangle(rotate * n_left,   (length/2.0f) * (rotate * c_left) + center,   rotate * orin_left,   length, length, dummy_mat);
    top =    Rectangle(rotate * n_top,    (length/2.0f) * (rotate * c_top) + center,    rotate * orin_top,    length, length, dummy_mat);
    bottom = Rectangle(rotate * n_bottom, (length/2.0f) * (rotate * c_bottom) + center, rotate * orin_bottom, length, length, dummy_mat);
}


SkyBox::~SkyBox() {}


// Returns color of texture at intersection.
Vector3f SkyBox::intersect(const Ray& r, Hit& hit)
{
    float tmin = 0.0f;

    // BETTER IDEA: just add a getTexture() to Material that does no shading on it!
    //              Give each square its texture then just call intersect! 

    if (front.intersect(r,hit,tmin))
    {
        return front_tex->operator()(hit.texCoord.x(), hit.texCoord.y());
    }
    else if (back.intersect(r,hit,tmin))
    {
        return back_tex->operator()(hit.texCoord.x(), hit.texCoord.y());
    }
    else if (right.intersect(r,hit,tmin))
    {
        return right_tex->operator()(hit.texCoord.x(), hit.texCoord.y());
    }
    else if (left.intersect(r,hit,tmin))
    {
        return left_tex->operator()(hit.texCoord.x(), hit.texCoord.y());
    }
    else if (top.intersect(r,hit,tmin))
    {
        return top_tex->operator()(hit.texCoord.x(), hit.texCoord.y());
    }
    else if (bottom.intersect(r,hit,tmin))
    {
        return bottom_tex->operator()(hit.texCoord.x(), hit.texCoord.y());
    }

    // Error: should have intersected some face of the skycube!
    assert(false);
    return Vector3f(1.0f);
}