#include <cstdint>
#include <cmath>
#include <utility>
#include <variant>
#include <vector>
#include <type_traits>
#include <myproject/vector_library.hpp>
#include <myproject/raytracing_library.hpp>


// NOLINTBEGIN(readability-identifier-length,bugprone-easily-swappable-parameters)
const auto BACKGROUND_COLOR = Color(0, 0, 0);

std::pair<double, double> intersect_ray_sphere(const Vector3d &ORIG, const Vector3d &DIR, const SceneObject &sphere);

std::pair<const SceneObject *, double> closest_intersection(const Vector3d &O,
  const Vector3d &D,
  const std::vector<SceneObject> &objects,
  const double t_min,
  const double t_max);

/**
 * Rechnet fuer jeden Punkt P die Lichtverhaeltnisse aus
 * @param lights Alle Lichter der Szene
 * @param P der Punkt
 * @param N vector abgehend von der oberflaeche des Objekts
 * @param V der vektor in die kamera, also wie stark das Licht in unser Auge rein reflektiert
 * @param specular Wie glaenzend das Objekt ist
 * @param objects Alle Objekte um zu schauen ob dieser Punkt im Schatten eines Objekts liegt
 */
double compute_lighting(const std::vector<Light> &lights,
  const Vector3d &P,
  const Vector3d &N, const Vector3d &V,
  const double specular,
  const std::vector<SceneObject> &objects);

Vector3d reflect_ray(const Vector3d& R, const Vector3d& N);

Color trace_ray(const Vector3d &O,
  const Vector3d &D,
  double t_min,
  double t_max,
  const std::vector<SceneObject> &objects,
  const std::vector<Light> &lights);



void raytrace(const RaytracingContext& ctx) { 
  const auto V_h = 1;
  const auto V_w = 1;

  const auto ORIG = Vector3d(0.F, 0.F, 0.F);
  for (auto y = (int16_t)(- ctx.height / 2); y < ctx.height / 2; y++) {
    for (auto x = (int16_t)(- ctx.width / 2); x < ctx.width / 2; x++) {
      // map canvas (pixel) coordinates to viewport coordinates
      const auto v_x = (double)x * ((double)V_w / (double)ctx.width);
      const auto v_y = (double)y * ((double)V_h / (double)ctx.height);
      // Der Punkt den wir gerade anschauen wollen
      const auto DIR = Vector3d(v_x, v_y, 1.F).norm();

      const auto t_min = 1.F;
      const auto t_max = (double)INFINITY;

      const auto color = trace_ray(ORIG, DIR, t_min, t_max, ctx.objects, ctx.lights);

      ctx.putPixelFct((double)x, (double)-y, color);
    }
  }
}

Color trace_ray(const Vector3d& O, const Vector3d& D, double t_min, double t_max, const std::vector<SceneObject>& objects, const std::vector<Light>& lights) {
  const auto [closest_sphere, closest_t] = closest_intersection(O, D, objects, t_min, t_max);

  // empty space
  if (closest_sphere == nullptr) { return BACKGROUND_COLOR; }

  // hier trifft der ray unser objekt
  const auto &P = O + (D * closest_t);
  // vektor quasi rechtwinklig abgehend vom objekt
  const auto N = P - closest_sphere->pos;
  const auto &color =
    closest_sphere->color * compute_lighting(lights, P, N.norm(), -D, closest_sphere->specular, objects);

  return color;
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

  return { (-b + sqrt(discriminant)) / (2 * a), (-b - sqrt(discriminant)) / (2 * a) };
}

double compute_lighting(const std::vector<Light> &lights,
  const Vector3d &P,
  const Vector3d &N,
  const Vector3d &V,
  const double specular,
  const std::vector<SceneObject>& objects)
{
  double i = 0.0F;
  for (const auto &light : lights) {
    std::visit(
      [&](auto &&l) {
        using T = std::decay_t<decltype(l)>;
        if constexpr (std::is_same_v<T, AmbientLight>) {
          i += l.intensity;
          return;
        } else {
          const auto [L, t_max] = [&]() -> std::pair<Vector3d, double> {
            if constexpr (std::is_same_v<T, PointLight>) {
              return { l.pos - P, 1.0 };
            } else {
              return { l.direction, INFINITY };
            }
          }();

          // Schatten
          const auto [shadow_sphere, shadow_t] =
            closest_intersection(P, L, objects, 0.001, t_max);
          if (shadow_sphere != nullptr) {
              return;
          }

          // Diffusion
          const auto n_dot = N.dot(L);
          if (n_dot > 0.0) { i += l.intensity * n_dot / (N.length() * L.length()); }

          // Specular
          const auto R = (N * (2 * N.dot(L))) - L;
          const auto r_dot_v = R.dot(V);
          if (r_dot_v > 0.F) {
            i += l.intensity * std::pow(r_dot_v / (R.length() * V.length()), specular);
          }
        }
      },
      light);
  }

  return i;
}

std::pair<const SceneObject *, double> closest_intersection(const Vector3d &O, const Vector3d &D,  const std::vector<SceneObject> &objects,
  const double t_min, const double t_max) {
  auto closest_t = (double)INFINITY;
  const SceneObject *closes_sphere = nullptr;
  for (const auto &object : objects) {
    const auto [intersection1, intersection2] = intersect_ray_sphere(O, D, object);
    if (intersection1 > t_min && intersection1 < t_max && intersection1 < closest_t) {
      closest_t = intersection1;
      closes_sphere = &object;
    }
    if (intersection2 > t_min && intersection2 < t_max && intersection2 < closest_t) {
      closest_t = intersection2;
      closes_sphere = &object;
    }
  }
  return { closes_sphere, closest_t };
}

Vector3d reflect_ray(const Vector3d& R, const Vector3d& N) { return N * 2 * N.dot(R) - R; }

// NOLINTEND(readability-identifier-length,bugprone-easily-swappable-parameters)