#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <fstream> // for creating empty files

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include <string.h>

using namespace std;

struct UserInput
{
  char* SCENE_FILE = nullptr;
  char* COLORED_FILE = nullptr;
  char* NORMALS_FILE = nullptr;
  char* DEPTH_FILE = nullptr;
  int WIDTH = 0;
  int HEIGHT = 0;
  float DEPTH_NEAR = 0;
  float DEPTH_FAR = 0;
} input;

Image* colored = nullptr;
Image* depth = nullptr;
Image* normals = nullptr;
SceneParser* scene = nullptr;
Camera* camera = nullptr;
Group* world_objects = nullptr;

Vector3f clampColor(const Vector3f& color);
void colorPixel(bool hitSomething, const Hit& hit, int row, int col, Ray ray);
float clampedDepth(float depthInput, float depthMin, float depthMax);
bool handleUserInput(int argc, char* argv[]);
Vector3f normalColor(const Vector3f& normal);

#include "bitmap_image.hpp"
int main(int argc, char* argv[])
{
  if (handleUserInput(argc, argv))
  {
    return 0;
  }

  scene = new SceneParser(input.SCENE_FILE);
  camera = scene->getCamera();
  world_objects = scene->getGroup();

  const float delta_x = 2.0f / input.WIDTH;
  const float delta_y = 2.0f / input.HEIGHT;
  const Vector2f image_bottom_left = Vector2f(-1.0f, -1.0f);
  const Vector2f delta_half = Vector2f(delta_x, delta_y)/2.0f;
  const Vector2f pixel00_center = image_bottom_left + delta_half;

  colored = new Image(input.WIDTH, input.HEIGHT);
  if (input.DEPTH_FILE != nullptr)
  {
    depth = new Image(input.WIDTH, input.HEIGHT);
  }
  if (input.NORMALS_FILE != nullptr)
  {
    normals = new Image(input.WIDTH, input.HEIGHT);
  }

  for (int row = 0; row < input.WIDTH; row++)
  {
    for (int col = 0; col < input.HEIGHT; col++)
    {
      float x_offset = col * delta_x;
      float y_offset = row * delta_y;
      Vector2f pixel = pixel00_center + Vector2f(x_offset, y_offset);
      Ray ray = camera->generateRay(pixel);

      Hit hit;
      bool hitSomething = world_objects->intersect(ray, hit, camera->getTMin());
      colorPixel(hitSomething, hit, row, col, ray);
    }
  }

  colored->SaveImage(input.COLORED_FILE);
  if (depth != nullptr)
  {
    depth->SaveImage(input.DEPTH_FILE);
  }
  if (normals != nullptr)
  {
    normals->SaveImage(input.NORMALS_FILE);
  }

  return 0;
}

void colorPixel(bool hitSomething, const Hit& hit, int row, int col, Ray ray)
{
  if (hitSomething) // hit something
  {
    Vector3f color(0.0f);

    for (int i = 0; i < scene->getNumLights(); i++)
    {
      Vector3f dirToLight;
      Vector3f lightColor;
      float distToLight;
      scene->getLight(i)->getIllumination(ray.pointAtParameter(hit.getT()), dirToLight, lightColor, distToLight);
      Vector3f diffuse_and_specular = hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor);
      color += diffuse_and_specular;
    }

    Vector3f ambient = scene->getAmbientLight() * hit.getMaterial()->getDiffuseColor();
    color += ambient;

    Vector3f colorClamped = clampColor(color);

    colored->SetPixel(col, row, colorClamped);
    if (normals != nullptr)
    {
      normals->SetPixel(col, row, normalColor(hit.getNormal()));
    }
    if (depth != nullptr)
    {
      float distance = (ray.pointAtParameter(hit.getT()) - ray.getOrigin()).abs();
      float clamped = clampedDepth(distance, input.DEPTH_NEAR, input.DEPTH_FAR);
      float range = input.DEPTH_FAR - input.DEPTH_NEAR;

      // near is white (1), far is black (0)
      float color = (1 - ((clamped - input.DEPTH_NEAR)/range));
      depth->SetPixel(col, row, Vector3f(color));
    }
  }
  else // hit nothing
  {
    colored->SetPixel(col, row, scene->getBackgroundColor());
    if (normals != nullptr)
    {
      normals->SetPixel(col, row, scene->getBackgroundColor());
    }
    if (depth != nullptr)
    {
      depth->SetPixel(col, row, Vector3f(0));
    }
  }
}

// true if error occurred
bool handleUserInput(int argc, char* argv[])
{
  if (argc == 1)
  {
    std::cout << "Command Line Parameters:\n-input <file>\n-output <file>\n-size <width> <height>\n-depth <file> <near> <far> -normals <file>" << std::endl;
    return true;
  }

  // ASSUMPTION: user is not an idiot (wait, I'm the user...)
  for (int argNum = 1; argNum < argc; ++argNum)
  {
    if (strcmp(argv[argNum], "-input") == 0)
    {
      argNum++;
      input.SCENE_FILE = argv[argNum];
    }
    else if (strcmp(argv[argNum], "-output") == 0)
    {
      argNum++;
      input.COLORED_FILE = argv[argNum];
    }
    else if (strcmp(argv[argNum], "-size") == 0)
    {
      argNum++;
      input.WIDTH = std::stoi(argv[argNum]);
      argNum++;
      input.HEIGHT = std::stoi(argv[argNum]);
    }
    else if (strcmp(argv[argNum], "-depth") == 0)
    {
      argNum++;
      input.DEPTH_FILE = argv[argNum];
      argNum++;
      input.DEPTH_NEAR = std::stof(argv[argNum]);
      argNum++;
      input.DEPTH_FAR = std::stof(argv[argNum]);
    }
    else if (strcmp(argv[argNum], "-normals") == 0)
    {
      std::cout << argv[argNum] << '\n';
      std::cout << argv[argNum + 1] << '\n';

      argNum++;
      input.NORMALS_FILE = argv[argNum];
    }
  }

  bool error = false;
  if (input.SCENE_FILE == nullptr)
  {
    std::cout << "Error: must pass input [-input <input-file>]" << std::endl;
    error = true;
  }
  if (input.COLORED_FILE == nullptr)
  {
    std::cout << "Error: must pass output [-output <output-file>]" << std::endl;
    error = true;
  }
  if (input.WIDTH <= 0 || input.HEIGHT <= 0)
  {
    std::cout << "Error: didn't pass image size or passed in invalid image dimensions [-size <width> <height>]" << std::endl;
    error = true;
  }

  if (input.COLORED_FILE != nullptr)
  {
    // Creates empty file if one does't exist.
    // Program won't work if it doesn't already exist.
    std::ofstream file(input.COLORED_FILE);
    file.close();
  }
  if (input.DEPTH_FILE != nullptr)
  {
      // Creates empty file if one does't exist.
    // Program won't work if it doesn't already exist.
    std::ofstream file(input.DEPTH_FILE);
    file.close();
  }
  if (input.NORMALS_FILE != nullptr)
  {
      // Creates empty file if one does't exist.
    // Program won't work if it doesn't already exist.
    std::ofstream file(input.NORMALS_FILE);
    file.close();
  }

  return error;
}

float clampedDepth(float depthInput, float depthMin, float depthMax)
{
  if (depthInput < depthMin)
  {
    return depthMin;
  }
  if (depthInput > depthMax)
  {
    return depthMax;
  }

  return depthInput;
}

Vector3f normalColor(const Vector3f& normal)
{
  float r = normal.x() >= 0.0f ? normal.x() : -1.0f * normal.x();
  float g = normal.y() >= 0.0f ? normal.y() : -1.0f * normal.y();
  float b = normal.z() >= 0.0f ? normal.z() : -1.0f * normal.z();

  return Vector3f(r, g, b);
}

Vector3f clampColor(const Vector3f& color)
{
  float rClamp = clamp(color.x(), 0.0f, 1.0f);
  float gClamp = clamp(color.y(), 0.0f, 1.0f);
  float bClamp = clamp(color.z(), 0.0f, 1.0f);

  return Vector3f(rClamp, gClamp, bClamp);
}