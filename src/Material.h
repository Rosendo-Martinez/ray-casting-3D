#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"
///TODO:
///Implement Shade function that uses ambient, diffuse, specular and texture
class Material
{
public:
	
  Material(const Vector3f& d_color, const Vector3f& s_color=Vector3f::ZERO, float s=0)
    : diffuseColor(d_color), specularColor(s_color), shininess(s)
  {}

  virtual ~Material()
  {}

  virtual Vector3f getDiffuseColor() const 
  { 
    return diffuseColor;
  }

  Vector3f Shade(const Ray& ray, const Hit& hit, const Vector3f& dirToLight, const Vector3f& lightColor)
  {
    float diffuse = clamp(Vector3f::dot(dirToLight, hit.getNormal()), 0.0f, 1.0f);
    return (diffuse * lightColor) * this->getDiffuseColor();
  }

  void loadTexture(const char * filename)
  {
    t.load(filename);
  }

protected:
  Vector3f diffuseColor;
  Vector3f specularColor;
  float shininess;
  Texture t;
};



#endif // MATERIAL_H
