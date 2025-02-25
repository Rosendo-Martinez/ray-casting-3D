#include "Group.h"


Group::Group() {}


Group::Group(int num_objects)
{
  // Create heap array of pointers to Object3D
  objects = new Object3D*[num_objects]();
  num = num_objects;
}


// WARNING: assumes user takes care of freeing Objects3D!
Group::~Group()
{
  delete[] objects;
}


bool Group::intersect(const Ray& r, Hit& h, float tmin)
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


void Group::addObject(int index, Object3D* obj)
{
  objects[index] = obj;
}


int Group::getGroupSize()
{ 
  return num;
}