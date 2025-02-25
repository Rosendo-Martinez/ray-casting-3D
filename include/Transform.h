#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "Object3D.h"
#include <iostream>


// A wrapper class for transformable objects: Sphere.
class Transform : public Object3D
{
public:
  Transform();

  Transform(const Matrix4f &m, Object3D *obj);
  ~Transform();

  virtual bool intersect(const Ray &r, Hit &h, float tmin);

protected:
  Object3D *o; // un-transformed object
  Matrix4f m;  // object to world space transformation
  Matrix4f m_inv;
  Matrix4f m_inv_trans;
};


#endif // TRANSFORM_H