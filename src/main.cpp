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
} userInput;

float clampedDepth(float depthInput, float depthMin, float depthMax);
bool handleUserInput(int argc, char* argv[]);

#include "bitmap_image.hpp"
int main(int argc, char* argv[])
{
  if (handleUserInput(argc, argv))
  {
    return 0;
  }

  Image image(userInput.imageWidth, userInput.imageHeight);
  SceneParser scene(userInput.inputFile);
  Camera* camera = scene.getCamera();
  Group* zaWarudo = scene.getGroup();
  const float deltaX = 2.0f / userInput.imageWidth;
  const float deltaY = 2.0f / userInput.imageHeight;
  const Vector2f pixel_00 = Vector2f(-1.0f, 1.0f) + Vector2f(deltaX, -deltaY)/2.0f;
  Image* imageDepth = nullptr;
  if (userInput.outputDepthFile != nullptr)
  {
    imageDepth = new Image(userInput.imageWidth, userInput.imageHeight);
  }

  for (int row = 0; row < userInput.imageWidth; row++)
  {
    for (int col = 0; col < userInput.imageHeight; col++)
    {
      Vector2f pixel = pixel_00 + (col * Vector2f(deltaX, 0.0f)) + (row * Vector2f(0.0f, -deltaY));
      Ray ray = camera->generateRay(pixel);
      Hit hit;

      bool hitSomething = zaWarudo->intersect(ray, hit, camera->getTMin());
      if (hitSomething) // hit something
      {
        image.SetPixel(row, col, hit.getMaterial()->getDiffuseColor());
      }
      else // hit nothing
      {
        image.SetPixel(row, col, scene.getBackgroundColor());
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
          imageDepth->SetPixel(row, col, Vector3f(color));
        }
        else
        {
          imageDepth->SetPixel(row, col, Vector3f(0));
        }
      }
    }
  }
  image.SaveImage(userInput.outputFile);

  if (imageDepth != nullptr)
  {
    imageDepth->SaveImage(userInput.outputDepthFile);
  }

  return 0;
}

// true if error occurred
bool handleUserInput(int argc, char* argv[])
{
  if (argc == 1)
  {
    std::cout << "Command Line Parameters:\n-input <file>\n-output <file>\n-size <width> <height>\n-depth <file> <near> <far>" << std::endl;
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