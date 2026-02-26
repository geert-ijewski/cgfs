#ifndef RAYTRACING_LIBRARY_HPP
#define RAYTRACING_LIBRARY_HPP

#include <vector>
#include <string>
#include <variant>
#include <cstdint>
#include <cmath>
#include <myproject/vector_library.hpp>

struct Color
{
  uint8_t r, g, b;
  Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

  /**
  * Multiply each channel (r,g,b) by the given intensity (double).
  * Clamp the computed double value into the [0, 255] range.
  * @param intensity how much more light this channel should have
  */
  Color operator*(const double &intensity) const
  {
    auto clamp_to_uint8 = [](double v) -> uint8_t {
      if (std::isnan(v)) v = 0.0;
      if (v < 0.0) v = 0.0;
      if (v > 255.0) v = 255.0;
      return static_cast<uint8_t>(std::lround(v));
    };

    double rr = static_cast<double>(r) * intensity;
    double gg = static_cast<double>(g) * intensity;
    double bb = static_cast<double>(b) * intensity;

    return Color(clamp_to_uint8(rr), clamp_to_uint8(gg), clamp_to_uint8(bb));
  }
};

struct SceneObject
{
  Vector3d pos;
  double radius;
  Color color;
  double specular;
  std::string name;
  SceneObject(Vector3d pos_, double radius_, Color color_, double specular, std::string name_) : pos(pos_), radius(radius_), color(color_), specular(specular), name(name_) {}
};

struct AmbientLight
{
  double intensity;
  explicit AmbientLight(double intensity_) : intensity(intensity_) {}
};

struct PointLight
{
  double intensity;
  Vector3d pos;
  PointLight(double intensity_, Vector3d pos_) : intensity(intensity_), pos(pos_) {}
};

struct DirectionalLight
{
  double intensity;
  Vector3d direction;
  DirectionalLight(double intensity_, Vector3d direction_) : intensity(intensity_), direction(direction_) {}
};

using Light = std::variant<AmbientLight, PointLight, DirectionalLight>;

typedef void (*PutPixel)(double x, double y, Color color);
struct RaytracingContext
{
  std::vector<SceneObject> objects;
  std::vector<Light> lights;
  PutPixel putPixelFct = nullptr;
  double height = 0;
  double width = 0;
};

void raytrace(const RaytracingContext &ctx);

#endif// !RAYTRACING_LIBRARY_HPP
