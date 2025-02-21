#ifndef SKYBOX_H
#define SKYBOX_H

#include "Object3D.h"

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

        float u = (horizontal_projected_distance + half_width) / width;
        float v = (vertical_projected_distance + half_height) / height;

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

class SkyBox : public Object3D
{
public:
    SkyBox() {}

    SkyBox(const Vector3f& center, float angle, Material* m) 
        : Object3D(m), center(center)
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
        Vector3f orin_bottom (-1,0,0);

        Matrix3f rotate (
            Vector3f(cos(angle), 0, -sin(angle)),
            Vector3f(0,1,0),
            Vector3f(sin(angle), 0, cos(angle))
        );

        std::cout << "Rotation Matrix: \n";
        rotate.print();
        std::cout << "------\n";

        float length = 1000; // not sure what a good length for SkyCube would be.

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

    virtual bool intersect(const Ray& r, Hit& h, float tmin)
    {   
        float u_width = 1.0f/4.0f;
        float v_height = 1.0f/3.0f;

        if (front.intersect(r,h,tmin))
        {
            std::cout << "Front face hit.\n";
            Vector2f bottom_left (1.0f/4.0f, 1.0f/3.0f);
            
            float u = h.texCoord.x() * u_width + bottom_left.x();
            float v = h.texCoord.y() * v_height + bottom_left.y();

            h.set(h.getT(), material, h.getNormal());
            h.setTexCoord(Vector2f(u,v));

            return true;
        }
        else if (back.intersect(r,h,tmin))
        {
            std::cout << "Back face hit.\n";
            Vector2f bottom_left (3.0f/4.0f, 1.0f/3.0f);
            
            float u = h.texCoord.x() * u_width + bottom_left.x();
            float v = h.texCoord.y() * v_height + bottom_left.y();

            h.set(h.getT(), material, h.getNormal());
            h.setTexCoord(Vector2f(u,v));
            return true;
        }
        else if (right.intersect(r,h,tmin))
        {
            std::cout << "Right face hit.\n";
            // re-map uv
            Vector2f bottom_left (2.0f/4.0f, 1.0f/3.0f);
            
            float u = h.texCoord.x() * u_width + bottom_left.x();
            float v = h.texCoord.y() * v_height + bottom_left.y();

            h.set(h.getT(), material, h.getNormal());
            h.setTexCoord(Vector2f(u,v));
            return true;
        }
        else if (left.intersect(r,h,tmin))
        {
            std::cout << "Left face hit.\n";
            // re-map uv
            Vector2f bottom_left (0.0f, 1.0f/3.0f);
            
            float u = h.texCoord.x() * u_width + bottom_left.x();
            float v = h.texCoord.y() * v_height + bottom_left.y();

            h.set(h.getT(), material, h.getNormal());
            h.setTexCoord(Vector2f(u,v));
            return true;
        }
        else if (top.intersect(r,h,tmin))
        {
            std::cout << "Top face hit.\n";
            // re-map uv
            Vector2f bottom_left (1.0f/4.0f, 2.0f/3.0f);
            
            float u = h.texCoord.x() * u_width + bottom_left.x();
            float v = h.texCoord.y() * v_height + bottom_left.y();

            h.set(h.getT(), material, h.getNormal());
            h.setTexCoord(Vector2f(u,v));
            return true;
        }
        else if (bottom.intersect(r,h,tmin))
        {
            std::cout << "Bottom face hit.\n";
            // re-map uv
            Vector2f bottom_left (1.0f/4.0f, 0.0f);
            
            float u = h.texCoord.x() * u_width + bottom_left.x();
            float v = h.texCoord.y() * v_height + bottom_left.y();

            h.set(h.getT(), material, h.getNormal());
            h.setTexCoord(Vector2f(u,v));
            return true;
        }

        return false;
    }

protected:
    Square top;
    Square bottom;
    Square left;
    Square right;
    Square front;
    Square back;
    Vector3f center;
};

#endif