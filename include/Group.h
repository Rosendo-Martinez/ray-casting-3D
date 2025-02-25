#ifndef GROUP_H
#define GROUP_H

#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"
#include <iostream>

using  namespace std;


class Group : public Object3D
{
public:
  Group();
  Group(int num_objects);

  // WARNING: assumes user takes care of freeing Objects3D!
  ~Group();

  virtual bool intersect(const Ray& r, Hit& h, float tmin);
  void addObject(int index, Object3D* obj);
  int getGroupSize();

private:
    Object3D** objects = nullptr;
    int num = 0;
};

#endif