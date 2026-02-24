// NOLINTBEGIN(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
#include <cstdint>
#include <cmath>
#include <utility>
//#include <SDL3/SDL_log.h>
#include <myproject/vector_library.hpp>
#include <myproject/raytracing_library.hpp>

std::pair<double, double>
  intersect_ray_sphere(const Vector3d &ORIG, const Vector3d &DIR, const SceneObject &sphere);

void raytrace(const RaytracingContext& ctx) { 
  const auto V_h = 1;
  const auto V_w = 1;

  const auto ORIG = Vector3d(0.F, 0.F, 0.F);
  for (auto y = (int16_t)(- ctx.height / 2); y < ctx.height / 2; y++) {// NOLINT(readability-identifier-length)
    for (auto x = (int16_t)(- ctx.width / 2); x < ctx.width / 2; x++) {// NOLINT(readability-identifier-length)
      // map canvas (pixel) coordinates to viewport coordinates
      const auto v_x = (double)x * ((double)V_w / (double)ctx.width);
      const auto v_y = (double)y * ((double)V_h / (double)ctx.height);
      const auto DIR = Vector3d(v_x, v_y, 1.F);

      const auto t_min = 1.F;
      const auto t_max = (double)INFINITY;

      auto closest_t = (double)INFINITY;
      const SceneObject *closes_sphere = nullptr;
      for (const auto &object : ctx.objects) {
        const auto [intersection1, intersection2] = intersect_ray_sphere(ORIG, DIR, object);
        if (intersection1 > t_min && intersection1 < t_max && intersection1 < closest_t) {
          closest_t = intersection1;
          closes_sphere = &object;
        }
        if (intersection2 > t_min && intersection2 < t_max && intersection2 < closest_t) {
          closest_t = intersection2;
          closes_sphere = &object;
        }
      }

      // empty space
      if (closes_sphere == nullptr) {
        continue;
      }
      // draw using canvas pixel coordinates (centered)
      ctx.putPixelFct((double)x, (double)-y, closes_sphere->color);
    }
  }
}

std::pair<double, double>
  intersect_ray_sphere(const Vector3d &ORIG, const Vector3d &DIR, const SceneObject &sphere) //NOLINT(bugprone-easily-swappable-parameters)
{
  const auto r = sphere.radius;// NOLINT(readability-identifier-length)
  const auto CO = ORIG - sphere.pos;// NOLINT(readability-identifier-length)

  const auto a = DIR.dot(DIR);// NOLINT(readability-identifier-length)
  const auto b = 2 * CO.dot(DIR);// NOLINT(readability-identifier-length)
  const auto c = CO.dot(CO) - (r * r);// NOLINT(readability-identifier-length)

  const auto discriminant = (b * b) - (4 * a * c);
  if (discriminant < 0) { return { INFINITY, INFINITY }; }

  return { (-b + sqrt(discriminant)) / 2 * a, (-b - sqrt(discriminant)) / 2 * a };
}
// NOLINTEND(cppcoreguidelines-pro-type-vararg,hicpp-vararg)