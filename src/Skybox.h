#ifndef SKYBOX_H
#define SKYBOX_H

#include "Object3D.h"
#include <cassert>

class Square : public Object3D
{   
public:
	Square() {}

	Square(const Vector3f& normal, const Vector3f& center, const Vector3f& orientation, float width, float height, Material* m) 
		: Object3D(m), normal(normal), center(center), width(width), height(height)
	{
        this->horizontal = orientation.normalized();
        this->vertical = Vector3f::cross(normal, this->horizontal).normalized();
        this->d = Vector3f::dot(center, normal);

        std::cout << "  center: "; center.print();
        std::cout << "  normal: "; normal.print();
        std::cout << "  orien: "; orientation.print();
        std::cout << "  dimensions: " << width << ' ' << height << '\n';
        std::cout << "  d: " << this->d << '\n';
    }

	~Square() {}

	virtual bool intersect(const Ray& r, Hit& h, float tmin)
	{
        // NOTE: MOST of this code copied & pasted from Plane class.
        //       Maybe instead should have just created a Plane instance, then uses that...

		float n_dot_rd = Vector3f::dot(normal, r.getDirection());

		// Ray and plane are parallel
		if (n_dot_rd == 0)
		{
			return false;
		}

		float n_dot_ro = Vector3f::dot(normal, r.getOrigin());
		float t = (d - n_dot_ro) / n_dot_rd;

		if (t < tmin || t > h.getT())
		{
			return false;
		}

        Vector3f p = r.pointAtParameter(t);

        float horizontal_projected_distance = Vector3f::dot(p - center, horizontal);
        float vertical_projected_distance = Vector3f::dot(p - center, vertical);

        float half_width = width / 2.0f;
        float half_height = height / 2.0f;

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

        // float u = (horizontal_projected_distance + half_width) / width;
        // float v = (vertical_projected_distance + half_height) / height;
        float u = (horizontal_projected_distance / width) + 0.5f;
        float v = (vertical_projected_distance / height) + 0.5f;

        if (u > 1.0f || u < 0.0f || v > 1.0f || v < 0.0f)
        {
            std::cout << "u,v = " << u << ','<< v << '\n';
        }

        assert(u >= 0.0f && u <= 1.0f);
        assert(v >= 0.0f && v <= 1.0f);

        h.setTexCoord(Vector2f(u,v));
		h.set(t, material, normal);
		return true;
	}

protected:
	Vector3f normal;
    Vector3f center;
    Vector3f horizontal;
    Vector3f vertical;
    float width;
    float height;
	float d;
};

class SkyBox
{
public:
    SkyBox() {}

    SkyBox(const Vector3f& center, float angle, Texture* front_tex, Texture* back_tex, Texture* right_tex, Texture* left_tex, Texture* top_tex, Texture* bottom_tex)
        : center(center), front_tex(front_tex), back_tex(back_tex), right_tex(right_tex), left_tex(left_tex), top_tex(top_tex), bottom_tex(bottom_tex)
    {
        std::cout << "Center: "; center.print();
        std::cout << "Angle: " << angle << '\n';

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

        Matrix3f rotate (
            Vector3f(cos(angle), 0, -sin(angle)),
            Vector3f(0,1,0),
            Vector3f(sin(angle), 0, cos(angle))
        );

        std::cout << "Rotation Matrix: \n";
        rotate.print();
        std::cout << "------\n";

        float length = 10000; // not sure what a good length for SkyCube would be.

        std::cout << "Front:\n";
        front =  Square(rotate * n_front,  (length/2.0f) * (rotate * c_front) + center,  rotate * orin_front,  length, length, nullptr);
        std::cout << "Back:\n";
        back =   Square(rotate * n_back,   (length/2.0f) * (rotate * c_back) + center,   rotate * orin_back,   length, length, nullptr);
        std::cout << "Right:\n";
        right =  Square(rotate * n_right,  (length/2.0f) * (rotate * c_right) + center,  rotate * orin_right,  length, length, nullptr);
        std::cout << "Left:\n";
        left =   Square(rotate * n_left,   (length/2.0f) * (rotate * c_left) + center,   rotate * orin_left,   length, length, nullptr);
        std::cout << "Top:\n";
        top =    Square(rotate * n_top,    (length/2.0f) * (rotate * c_top) + center,    rotate * orin_top,    length, length, nullptr);
        std::cout << "Bottom:\n";
        bottom = Square(rotate * n_bottom, (length/2.0f) * (rotate * c_bottom) + center, rotate * orin_bottom, length, length, nullptr);
    }

    ~SkyBox() {}

    /**
     * Intersects the skybox. Returns intersected texture pixel color.
     */
    void intersect(const Ray& r, Vector3f& color, Vector3f& normal)
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

        std::cout << "ERROR: should have intersected some face of skybox!\n";
        color = Vector3f(1.0f);
        normal = Vector3f(0.0f);

        return;
    }

protected:
    Square top;
    Square bottom;
    Square left;
    Square right;
    Square front;
    Square back;
    Vector3f center;

    Texture* front_tex;
    Texture* back_tex;
    Texture* right_tex;
    Texture* left_tex;
    Texture* top_tex;
    Texture* bottom_tex;
};

#endif