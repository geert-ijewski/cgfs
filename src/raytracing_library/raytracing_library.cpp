#include <cstdint>
#include <cmath>
#include <utility>
#include <variant>
#include <vector>
#include <type_traits>
#include <myproject/vector_library.hpp>
#include <myproject/raytracing_library.hpp>

std::pair<double, double>
  intersect_ray_sphere(const Vector3d &ORIG, const Vector3d &DIR, const SceneObject &sphere);

// NOLINTBEGIN(readability-identifier-length,bugprone-easily-swappable-parameters)
/**
 * Rechnet fuer jeden Punkt P die Lichtverhaeltnisse aus
 * @param lights Alle Lichter der Szene
 * @param P der Punkt
 * @param N vector abgehend von der oberflaeche des Objekts
 */
double compute_lighting(const std::vector<Light> &lights, const Vector3d &P, const Vector3d &N);


void raytrace(const RaytracingContext& ctx) { 
  const auto V_h = 1;
  const auto V_w = 1;

  const auto ORIG = Vector3d(0.F, 0.F, 0.F);
  for (auto y = (int16_t)(- ctx.height / 2); y < ctx.height / 2; y++) {
    for (auto x = (int16_t)(- ctx.width / 2); x < ctx.width / 2; x++) {
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
      const auto &P = ORIG + DIR * closest_t;
      const auto N = P - closes_sphere->pos;
      const auto &color = closes_sphere->color * compute_lighting(ctx.lights, P, N.norm());

      ctx.putPixelFct((double)x, (double)-y, color);
    }
  }
}

std::pair<double, double>
  intersect_ray_sphere(const Vector3d &ORIG, const Vector3d &DIR, const SceneObject &sphere)
{
  const auto r = sphere.radius;
  const auto CO = ORIG - sphere.pos;

  const auto a = DIR.dot(DIR);
  const auto b = 2 * CO.dot(DIR);
  const auto c = CO.dot(CO) - (r * r);

  const auto discriminant = (b * b) - (4 * a * c);
  if (discriminant < 0) { return { INFINITY, INFINITY }; }

  return { (-b + sqrt(discriminant)) / 2 * a, (-b - sqrt(discriminant)) / 2 * a };
}


double compute_lighting(const std::vector<Light> &lights, const Vector3d &P, const Vector3d &N)
{
  double i = 0.0F;
  for (const auto &light : lights) {
    std::visit(
      [&](auto &&l) {
        using T = std::decay_t<decltype(l)>;
        if constexpr (std::is_same_v<T, AmbientLight>) {
          i += l.intensity;
        } else if constexpr (std::is_same_v<T, PointLight>) {
          const auto L = l.pos - P;
          const auto n_dot = N.dot(L);
          if (n_dot > 0.0) { i += l.intensity * n_dot / (N.length() * L.length()); }
        } else if constexpr (std::is_same_v<T, DirectionalLight>) {
          const auto L = l.direction;
          const auto n_dot = N.dot(L);
          if (n_dot > 0.0) { i += l.intensity * n_dot / (N.length() * L.length()); }
        }
      },
      light);
  }

  return i;
}
// NOLINTEND(readability-identifier-length,bugprone-easily-swappable-parameters)