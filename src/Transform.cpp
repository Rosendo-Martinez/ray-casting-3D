#include "Transform.h"


Transform::Transform() {}


Transform::Transform(const Matrix4f &m, Object3D *obj)
  : o(obj), m(m), m_inv(m.inverse()), m_inv_trans(m.inverse().transposed())
{
}


Transform::~Transform() {}


bool Transform::intersect(const Ray &r, Hit &h, float tmin)
{
  Vector4f rd_ws = Vector4f(r.getDirection(), 0.0f);
  Vector4f ro_ws = Vector4f(r.getOrigin(), 1.0f);

  // Ray in object space
  Vector4f rd_os = m_inv * rd_ws;
  Vector4f ro_os = m_inv * ro_ws;

  // Object-ray intersection in object space
  if (!o->intersect(Ray(ro_os.xyz(), rd_os.xyz()), h, tmin))
  {
    return false;
  }

  // Transform normal to world space
  Vector4f normal_os = Vector4f(h.getNormal(), 0.0f);
  Vector3f normal_ws = (m_inv_trans * normal_os).xyz().normalized();
  h.set(h.getT(), h.getMaterial(), normal_ws, h.hasTex);

  return true;
}