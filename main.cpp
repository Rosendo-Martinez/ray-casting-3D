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

float clampedDepth ( float depthInput, float depthMin , float depthMax);

#include "bitmap_image.hpp"
int main( int argc, char* argv[] )
{
  // Fill in your implementation here.

  // This loop loops over each of the input arguments.
  // argNum is initialized to 1 because the first
  // "argument" provided to the program is actually the
  // name of the executable (in our case, "a4").

  // User input
  std::string OUTPUT;
  std::string INPUT;
  int WIDTH = 0;
  int HEIGHT = 0;

  for( int argNum = 1; argNum < argc; ++argNum )
    {
      std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;

      // if (std::string(argv[argNum]) == "-input")
      if (strcmp(argv[argNum], "-input") == 0)
      {
        INPUT = argv[argNum + 1];
      }
      // else if (argv[argNum] == "-output")
      else if (strcmp(argv[argNum], "-output") == 0)
      {
        OUTPUT = argv[argNum + 1];
      }
      // else if (argv[argNum] == "-size")
      else if (strcmp(argv[argNum], "-size") == 0)
      {
        WIDTH = std::stoi(argv[argNum + 1]);
        HEIGHT = std::stoi(argv[argNum + 2]);
      }
    }
    
    std::cout << INPUT << '\n';
  // Creates empty file if one doesn't exist.
  if (OUTPUT.size() != 0)
  {
    std::ofstream file(OUTPUT);
    file.close();
  }

  // First, parse the scene using SceneParser.
  // Then loop over each pixel in the image, shooting a ray
  // through that pixel and finding its intersection with
  // the scene.  Write the color at the intersection to that
  // pixel in your output image.

  Image image(WIDTH, HEIGHT);
  // std::cout << INPUT.size() << '\n';
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
      
        std::cout << "HELLO?\n";
      if (zaWarudo->intersect(ray, hit, camera->getTMin())) // hit something
      {
        image.SetPixel(row, col, hit.getMaterial()->getDiffuseColor());
      }
      else
      {
        image.SetPixel(row, col, scene.getBackgroundColor());
      }
    }
  }

  std::cout << OUTPUT.size() << '\n';

  image.SaveImage(OUTPUT.c_str());
 
  ///TODO: below demonstrates how to use the provided Image class
  ///Should be removed when you start
  // Vector3f pixelColor (1.0f,0,0);
  // //width and height
  // Image image( 10 , 15 );
  // image.SetPixel( 5,5, pixelColor );
  // image.SaveImage("hello.bmp");
  return 0;
}

