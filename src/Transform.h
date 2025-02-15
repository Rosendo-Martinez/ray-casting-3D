#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "Object3D.h"

///TODO: implement this class
///So that the intersect function first transforms the ray
///Add more fields as necessary
class Transform : public Object3D
{
public:
  Transform() {}

  Transform(const Matrix4f& m, Object3D* obj)
    : o(obj), m(m), m_inv(m.inverse())
  {
    std::cout << "Hello from Transform constructor!\n";

    this->m.print();
    m_inv.print();
  }

  ~Transform()
  {}

  virtual bool intersect(const Ray& r, Hit& h, float tmin)
  {
    // Ray from world to object space
    Vector4f ray_dir_os = m_inv * Vector4f(r.getDirection(), 0.0f);
    Vector4f ray_org_os = m_inv * Vector4f(r.getOrigin(), 1.0f);
    Ray rayOS (ray_org_os.xyz(), ray_dir_os.xyz());

    // ray_dir_os.print();
    // ray_org_os.print();

    // std::cout << "Hello, from transform?\n";
    // Object-ray intersection in object space
    // std::cout << "Transform int\n";
    // std::cout << "Done w/ Transform int\n";
    bool isHit = o->intersect(rayOS, h, tmin);
    if (isHit)
    {
      // std::cout << "Tranform hit\n";

      // Transform normal to world space
      Vector4f normalWS = (m_inv.transposed() * Vector4f(h.getNormal(), 0.0f)).normalized();
      h.set(h.getT(), h.getMaterial(), normalWS.xyz());
    }

    return isHit;
  }

protected:
  Object3D* o; //un-transformed object
  Matrix4f m; // object to world space transformation
  Matrix4f m_inv;
};

#endif //TRANSFORM_H
