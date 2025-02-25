#include "Material.h"


Material::Material(const Vector3f& d_color, const Vector3f& s_color, float s)
  : diffuseColor(d_color), specularColor(s_color), shininess(s)
{}


Material::~Material()
{}


Vector3f Material::getDiffuseColor() const 
{ 
  return diffuseColor;
}


// Uses diffuse, specular and texture for shading.
Vector3f Material::Shade(const Ray& ray, const Hit& hit, const Vector3f& dirToLight, const Vector3f& lightColor)
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

    return (diffuse * lightColor * texture) + (specular * lightColor * specularColor);
  }
  else // no texture, use diffuse color
  {
    return (diffuse * lightColor * diffuseColor) + (specular * lightColor * specularColor);
  }
}


void Material::loadTexture(const char * filename)
{
  tex.load(filename);
}


void Material::loadBumps(const char * filename)
{
  bum.load(filename);
}