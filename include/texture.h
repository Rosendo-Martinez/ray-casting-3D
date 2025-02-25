#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include "bitmap_image.hpp"
#include "Vector3f.h"

///@brief helper class that stores a texture and faciliates lookup
///assume 4byte RGBA image data

// QUESTION: The above comment may be wrong, not sure. 
// The bitmap_image.hpp file says it can only handle 3Byte RGB images.

class Texture{
public:
  Texture();
  bool valid();
  void load(const char * filename);
  void operator()(int x, int y,  unsigned char * color);
  ///@param x assumed to be between 0 and 1
  Vector3f operator()(float x, float y);
  ~Texture();
private:
  bitmap_image * bimg;
  int width , height;
};
#endif
