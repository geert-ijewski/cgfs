#ifndef RAYTRACING_LIBRARY_HPP
#define RAYTRACING_LIBRARY_HPP

#include <vector>
#include <string>
#include <myproject/vector_library.hpp>

struct Color
{
  uint8_t r, g, b;
  Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
};

struct SceneObject
{
  Vector3d pos;
  double radius;
  Color color;
  std::string name;
  SceneObject(Vector3d pos_, double radius_, Color color_, std::string name_) : pos(pos_), radius(radius_), color(color_), name(name_) {}
};

typedef void (*PutPixel)(double x, double y, Color color);
struct RaytracingContext
{
  std::vector<SceneObject> objects;
  PutPixel putPixelFct = nullptr;
  double height = 0;
  double width = 0;
};

void raytrace(const RaytracingContext &ctx);

#endif// !RAYTRACING_LIBRARY_HPP
