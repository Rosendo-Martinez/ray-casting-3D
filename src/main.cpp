#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string.h>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include "VecUtils.h"
#include "bitmap_image.hpp"


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
}
input;

Image* colored = nullptr;
Image* depth = nullptr;
Image* normals = nullptr;
SceneParser* scene = nullptr;
Camera* camera = nullptr;
Group* world_objects = nullptr;

Vector3f get_pixel_depth_color(const Ray& ray, const Hit& hit);
Vector3f get_pixel_shaded_color(const Ray& ray, const Hit& hit);
void drawPixel(bool hitSomething, const Hit& hit, int row, int col, Ray ray);
float clampf(float depthInput, float depthMin, float depthMax);
bool handleUserInput(int argc, char* argv[]);
Vector3f get_normal_color(const Vector3f& normal);


int main(int argc, char* argv[])
{
    if (handleUserInput(argc, argv))
    {
        return 0;
    }

    scene = new SceneParser(input.SCENE_FILE);
    camera = scene->getCamera();
    world_objects = scene->getGroup();

    // Create images to draw on to.
    colored = new Image(input.WIDTH, input.HEIGHT);
    depth = new Image(input.WIDTH, input.HEIGHT);
    normals = new Image(input.WIDTH, input.HEIGHT);


    const float delta_x = 2.0f / input.WIDTH;
    const float delta_y = 2.0f / input.HEIGHT;
    const Vector2f image_bottom_left = Vector2f(-1.0f, -1.0f);
    const Vector2f delta_half = Vector2f(delta_x, delta_y)/2.0f;
    const Vector2f pixel00_center = image_bottom_left + delta_half;

    // Render loop.
    // Sends rays through the center of each pixel.
    for (int row = 0; row < input.WIDTH; row++)
    {
        for (int col = 0; col < input.HEIGHT; col++)
        {
            Vector2f pixel = pixel00_center + Vector2f(col * delta_x, row * delta_y);
            Ray ray = camera->generateRay(pixel);
            Hit hit;

            bool hitSomething = world_objects->intersect(ray, hit, camera->getTMin());
            drawPixel(hitSomething, hit, row, col, ray);
        }
    }


    // Save rendered images to user input files.
    colored->SaveImage(input.COLORED_FILE);
    depth->SaveImage(input.DEPTH_FILE);
    normals->SaveImage(input.NORMALS_FILE);

    return 0;
}


// Helper Functions: rendering ---------------------------------------------------------------------------------------------


void drawPixel(bool hitSomething, const Hit& hit, int row, int col, Ray ray)
{
    if (hitSomething) // hit
    {
        // Save to image
        colored->SetPixel(col, row, get_pixel_shaded_color(ray, hit));
        normals->SetPixel(col, row, get_normal_color(hit.getNormal()));
        depth->SetPixel(col, row, get_pixel_depth_color(ray, hit));
    }
    else // no hit
    {
        SkyBox* skybox = scene->getSkyBox();
        if (skybox != nullptr) // skybox & no hit
        {
            Hit skybox_hit;

            colored->SetPixel(col, row, skybox->intersect(ray, skybox_hit));
            normals->SetPixel(col, row, get_normal_color(skybox_hit.getNormal()));
            depth->SetPixel(col, row, get_pixel_depth_color(ray, skybox_hit));
        }
        else // no skybox & no hit
        {
            colored->SetPixel(col, row, scene->getBackgroundColor());
            normals->SetPixel(col, row, scene->getBackgroundColor());
            depth->SetPixel(col, row, Vector3f(0));
        }
    }
}


float clampf(float value, float min, float max)
{
    if (value < min)
    {
        return min;
    }
    if (value > max)
    {
        return max;
    }

    return value;
}


Vector3f get_normal_color(const Vector3f& normal)
{
    float r = normal.x() >= 0.0f ? normal.x() : -1.0f * normal.x();
    float g = normal.y() >= 0.0f ? normal.y() : -1.0f * normal.y();
    float b = normal.z() >= 0.0f ? normal.z() : -1.0f * normal.z();

    return Vector3f(r, g, b);
}


Vector3f get_pixel_shaded_color(const Ray& ray, const Hit& hit)
{
    Vector3f pixel_color(0.0f);

    // Specular & diffuse shading
    for (int i = 0; i < scene->getNumLights(); i++)
    {
        // Light source data
        Vector3f dirToLight;
        Vector3f lightColor;
        float distToLight;
        scene->getLight(i)->getIllumination(ray.pointAtParameter(hit.getT()), dirToLight, lightColor, distToLight);

        Vector3f material_shaded = hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor);
        pixel_color += material_shaded;
    }

    // Ambient shading
    pixel_color += scene->getAmbientLight() * hit.getMaterial()->getDiffuseColor();

    return VecUtils::clamp(pixel_color, 0.0f, 1.0f);
}


Vector3f get_pixel_depth_color(const Ray& ray, const Hit& hit)
{
    float distance = (ray.pointAtParameter(hit.getT()) - ray.getOrigin()).abs();
    float clamped = clampf(distance, input.DEPTH_NEAR, input.DEPTH_FAR);
    float range = input.DEPTH_FAR - input.DEPTH_NEAR;
    // near is white (1), far is black (0)
    float color = (1 - ((clamped - input.DEPTH_NEAR)/range));

    return Vector3f(color);
}


// Helper Functions: user input --------------------------------------------------------------------------------------------------


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
    if (input.DEPTH_FILE == nullptr)
    {
        std::cout << "Error: must pass depth file name [-depth <file> <near> <far>]" << std::endl;
        error =  true;
    }
    if (input.NORMALS_FILE == nullptr)
    {
        std::cout << "Error: must pass normals file name [-normals <file>]" << std::endl;
        error = true;
    }
    if (input.WIDTH <= 0 || input.HEIGHT <= 0)
    {
        std::cout << "Error: didn't pass image size or passed in invalid image dimensions [-size <width> <height>]" << std::endl;
        error = true;
    }

    if (!error)
    {
        // Creates empty files to save rendered image to.
        std::ofstream colored_file(input.COLORED_FILE);
        colored_file.close();
        std::ofstream depth_file(input.DEPTH_FILE);
        depth_file.close();
        std::ofstream normals_file(input.NORMALS_FILE);
        normals_file.close();
    }

    return error;
}