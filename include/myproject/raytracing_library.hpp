#ifndef RAYTRACING_LIBRARY_HPP
#define RAYTRACING_LIBRARY_HPP

#include <vector>
#include <myproject/vector_library.hpp>

struct Color
{
  uint8_t r, g, b;
  Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
};

struct SceneObject
{
  Vector3d pos;
  Color color;
  SceneObject(Vector3d pos_, Color color_) : pos(pos_), color(color_) {}
};

typedef void (*PutPixel)(int16_t x, int16_t y, Color color);
struct RaytracingContext
{
  std::vector<SceneObject> objects;
  PutPixel putPixelFct = nullptr;
  int16_t height = 0;
  int16_t width = 0;
};

void raytrace(const RaytracingContext &ctx);

#endif// !RAYTRACING_LIBRARY_HPP
