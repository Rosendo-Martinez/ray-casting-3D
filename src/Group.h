#ifndef GROUP_H
#define GROUP_H


#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"
#include <iostream>

using  namespace std;


class Group:public Object3D
{
public:
  Group() {}
	
  Group(int num_objects)
  {
    // Create heap array of pointers to Object3D
    objects = new Object3D*[num_objects]();
    num = num_objects;
  }

  // WARNING: assumes user takes care of freeing Objects3D!
  ~Group()
  {
    delete[] objects;
  }

  virtual bool intersect(const Ray& r, Hit& h, float tmin)
  {
    bool hitSomething = false;
    for (int i = 0; i < num; i++)
    {
      if (objects[i]->intersect(r, h, tmin))
      {
        hitSomething = true;
      }
    }

    return hitSomething;
  }
	
  void addObject(int index, Object3D* obj)
  {
    objects[index] = obj;
  }

  int getGroupSize()
  { 
    return num;
  }

 private:
    Object3D** objects = nullptr;
    int num = 0;
    
};

#endif
	
