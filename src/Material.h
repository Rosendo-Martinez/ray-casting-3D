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
    Vector3f reflected = (ray.getDirection() - 2 * (Vector3f::dot(ray.getDirection(), hit.getNormal())) * hit.getNormal()).normalized();

    float specular = pow(clamp(Vector3f::dot(reflected, dirToLight), 0.0f, 1.0f), shininess);
    float diffuse = clamp(Vector3f::dot(dirToLight, hit.getNormal()), 0.0f, 1.0f);

    if (hit.hasTex)
    {
      Vector2f tex = hit.texCoord;
      return (diffuse * lightColor) * t(tex.x(), tex.y()) + (specular * lightColor) * specularColor;
    }

    return (diffuse * lightColor) * this->getDiffuseColor() + (specular * lightColor) * specularColor;
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
