#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"
#include "HeightMap.h"

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

  // Uses diffuse, specular and texture for shading.
  Vector3f Shade(const Ray& ray, const Hit& hit, const Vector3f& dirToLight, const Vector3f& lightColor)
  {
    Vector3f rd = ray.getDirection();
    Vector3f n = hit.getNormal();
    float rd_dot_n = Vector3f::dot(rd, n);

    // reflected vector
    Vector3f reflected = rd - (2 * rd_dot_n * n);
    reflected.normalize();

    float specular = pow(clamp(Vector3f::dot(reflected, dirToLight), 0.0f, 1.0f), shininess);
    float diffuse = clamp(Vector3f::dot(dirToLight, hit.getNormal()), 0.0f, 1.0f);

    if (tex.valid()) // has texture
    {
      Vector3f texture = tex(hit.texCoord.x(), hit.texCoord.y());

      // return (diffuse * lightColor * texture) + (specular * lightColor * specularColor);
      return texture;
    }
    else // no texture, use diffuse color
    {
      return (diffuse * lightColor * diffuseColor) + (specular * lightColor * specularColor);
    }
  }

  void loadTexture(const char * filename)
  {
    tex.load(filename);
  }

  void loadBumps(const char * filename)
  {
    bum.load(filename);
  }

protected:
  Vector3f diffuseColor;
  Vector3f specularColor;
  float shininess;
  Texture tex;
  HeightMap bum;
};


#endif // MATERIAL_H
