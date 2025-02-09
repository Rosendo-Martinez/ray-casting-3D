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

float clampedDepth(float depthInput, float depthMin, float depthMax);

#include "bitmap_image.hpp"
int main(int argc, char* argv[])
{
  // User input
  std::string OUTPUT;
  std::string INPUT;
  int WIDTH = 0;
  int HEIGHT = 0;

  for( int argNum = 1; argNum < argc; ++argNum )
  {
    std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;

    if (strcmp(argv[argNum], "-input") == 0)
    {
      INPUT = argv[argNum + 1];
    }
    else if (strcmp(argv[argNum], "-output") == 0)
    {
      OUTPUT = argv[argNum + 1];
    }
    else if (strcmp(argv[argNum], "-size") == 0)
    {
      WIDTH = std::stoi(argv[argNum + 1]);
      HEIGHT = std::stoi(argv[argNum + 2]);
    }
  }

  // Creates empty file if one doesn't exist.
  if (OUTPUT.size() != 0)
  {
    std::ofstream file(OUTPUT);
    file.close();
  }

  Image image(WIDTH, HEIGHT);
  SceneParser scene(INPUT.c_str());
  Camera* camera = scene.getCamera();
  Group* zaWarudo = scene.getGroup();

  const float deltaX = 2.0f / WIDTH;
  const float deltaY = 2.0f / HEIGHT;
  const Vector2f pixel_00 = Vector2f(-1.0f, 1.0f) - Vector2f(deltaX, deltaY);

  for (int row = 0; row < WIDTH; row++)
  {
    for (int col = 0; col < HEIGHT; col++)
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
    }
  }
  image.SaveImage(OUTPUT.c_str());

  return 0;
}
