#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "Ray.h"
#include "Hit.h"
#include "texture.h"
#include "HeightMap.h"


class Material
{
public:
  Material(const Vector3f& d_color, const Vector3f& s_color=Vector3f::ZERO, float s=0);
  virtual ~Material();

  virtual Vector3f getDiffuseColor() const;

  // Uses diffuse, specular and texture for shading.
  Vector3f Shade(const Ray& ray, const Hit& hit, const Vector3f& dirToLight, const Vector3f& lightColor);
  void loadTexture(const char * filename);
  void loadBumps(const char * filename);

  HeightMap bum;

protected:
  Vector3f diffuseColor;
  Vector3f specularColor;
  float shininess;
  Texture tex;
};


#endif // MATERIAL_H
