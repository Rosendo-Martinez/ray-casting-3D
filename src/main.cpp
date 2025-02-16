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
  char* outputFile = nullptr;
  char* inputFile = nullptr;
  int imageWidth = 0;
  int imageHeight = 0;
  char* outputDepthFile = nullptr;
  float depthNear = 0;
  float depthFar = 0;
  char* outputNormals = nullptr;
} userInput;

Image* colored = nullptr;
Image* depth = nullptr;
Image* normals = nullptr;
SceneParser* scene = nullptr;
Camera* camera = nullptr;
Group* world_objects = nullptr;

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

  scene = new SceneParser(userInput.inputFile);
  camera = scene->getCamera();
  world_objects = scene->getGroup();

  const float delta_x = 2.0f / userInput.imageWidth;
  const float delta_y = 2.0f / userInput.imageHeight;
  const Vector2f pixel_bottom_left = Vector2f(-1.0f, -1.0f) + Vector2f(delta_x, delta_y)/2.0f;

  colored = new Image(userInput.imageWidth, userInput.imageHeight);
  if (userInput.outputDepthFile != nullptr)
  {
    depth = new Image(userInput.imageWidth, userInput.imageHeight);
  }
  if (userInput.outputNormals != nullptr)
  {
    normals = new Image(userInput.imageWidth, userInput.imageHeight);
  }

  for (int row = 0; row < userInput.imageWidth; row++)
  {
    for (int col = 0; col < userInput.imageHeight; col++)
    {
      Vector2f pixel = pixel_bottom_left + (col * Vector2f(delta_x, 0.0f)) + (row * Vector2f(0.0f, delta_y));
      Ray ray = camera->generateRay(pixel);
      Hit hit;

      bool hitSomething = world_objects->intersect(ray, hit, camera->getTMin());
      colorPixel(hitSomething, hit, row, col, ray);
    }
  }
  colored->SaveImage(userInput.outputFile);

  if (depth != nullptr)
  {
    depth->SaveImage(userInput.outputDepthFile);
  }
  if (normals != nullptr)
  {
    normals->SaveImage(userInput.outputNormals);
  }

  return 0;
}

void colorPixel(bool hitSomething, const Hit& hit, int row, int col, Ray ray)
{
  if (hitSomething) // hit something
  {
    Vector3f dirToLight;
    Vector3f lightColor;
    float distToLight;
    scene->getLight(0)->getIllumination(ray.pointAtParameter(hit.getT()), dirToLight, lightColor, distToLight);
    Vector3f ambient = scene->getAmbientLight() * hit.getMaterial()->getDiffuseColor();
    Vector3f color = hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor) + ambient;
    float rClamp = color.x() > 1.0f ? 1.0f : color.x();
    float gClamp = color.y() > 1.0f ? 1.0f : color.y();
    float bClamp = color.z() > 1.0f ? 1.0f : color.z();
    colored->SetPixel(col, row, Vector3f(rClamp, gClamp, bClamp));

    if (normals != nullptr)
    {
      normals->SetPixel(col, row, normalColor(hit.getNormal()));
    }
  }
  else // hit nothing
  {
    colored->SetPixel(col, row, scene->getBackgroundColor());

    if (normals != nullptr)
    {
      normals->SetPixel(col, row, scene->getBackgroundColor());
    }
  }

  if (userInput.outputDepthFile != nullptr)
  {
    if (hitSomething)
    {
      float distance = (ray.pointAtParameter(hit.getT()) - ray.getOrigin()).abs();
      float clamped = clampedDepth(distance, userInput.depthNear, userInput.depthFar);
      float range = userInput.depthFar - userInput.depthNear;

      // near is white (1), far is black (0)
      float color = (1 - ((clamped - userInput.depthNear)/range));
      depth->SetPixel(col, row, Vector3f(color));

    }
    else
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
      userInput.inputFile = argv[argNum];
    }
    else if (strcmp(argv[argNum], "-output") == 0)
    {
      argNum++;
      userInput.outputFile = argv[argNum];
    }
    else if (strcmp(argv[argNum], "-size") == 0)
    {
      argNum++;
      userInput.imageWidth = std::stoi(argv[argNum]);
      argNum++;
      userInput.imageHeight = std::stoi(argv[argNum]);
    }
    else if (strcmp(argv[argNum], "-depth") == 0)
    {
      argNum++;
      userInput.outputDepthFile = argv[argNum];
      argNum++;
      userInput.depthNear = std::stof(argv[argNum]);
      argNum++;
      userInput.depthFar = std::stof(argv[argNum]);
    }
    else if (strcmp(argv[argNum], "-normals") == 0)
    {
      std::cout << argv[argNum] << '\n';
      std::cout << argv[argNum + 1] << '\n';

      argNum++;
      userInput.outputNormals = argv[argNum];
    }
  }

  bool error = false;
  if (userInput.inputFile == nullptr)
  {
    std::cout << "Error: must pass input [-input <input-file>]" << std::endl;
    error = true;
  }
  if (userInput.outputFile == nullptr)
  {
    std::cout << "Error: must pass output [-output <output-file>]" << std::endl;
    error = true;
  }
  if (userInput.imageWidth <= 0 || userInput.imageHeight <= 0)
  {
    std::cout << "Error: didn't pass image size or passed in invalid image dimensions [-size <width> <height>]" << std::endl;
    error = true;
  }

  if (userInput.outputFile != nullptr)
  {
    // Creates empty file if one does't exist.
    // Program won't work if it doesn't already exist.
    std::ofstream file(userInput.outputFile);
    file.close();
  }
  if (userInput.outputDepthFile != nullptr)
  {
      // Creates empty file if one does't exist.
    // Program won't work if it doesn't already exist.
    std::ofstream file(userInput.outputDepthFile);
    file.close();
  }
  if (userInput.outputNormals != nullptr)
  {
      // Creates empty file if one does't exist.
    // Program won't work if it doesn't already exist.
    std::ofstream file(userInput.outputNormals);
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