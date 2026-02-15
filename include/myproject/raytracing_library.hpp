#ifndef RAYTRACING_LIBRARY_HPP
#define RAYTRACING_LIBRARY_HPP

struct Color
{
  uint8_t r, g, b;
  Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
};

struct SceneObject
{
  Vector3d pos;
  Color color;
};

typedef int (*PutPixel)(int16_t x, int16_t y, Color color);
struct RaytracingContext
{
  std::vector<SceneObject> objects;
  PutPixel putPixelFct;
};

#endif// !RAYTRACING_LIBRARY_HPP
