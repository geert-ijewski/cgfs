#include <cstdint>
#include <myproject/raytracing_library.hpp>


void raytrace(const RaytracingContext& ctx) { 
  // viewport definition, ich machs erstmal genau so gross wie den canvas
  const int16_t V_h = ctx.height;
  const int16_t V_w = ctx.width;

  for (int16_t y = 0; y < ctx.height; y++) { // NOLINT(readability-identifier-length)
    for (int16_t x = 0; x < ctx.width; x++) {// NOLINT(readability-identifier-length)
      const auto v_x = (int16_t)(x * (int16_t)(V_h / ctx.height));
      const auto v_y = (int16_t)(y * (int16_t)(V_w / ctx.width));

      for (const auto& object : ctx.objects) { 
        if (object.pos.x == v_x && object.pos.y == v_y) { 
            ctx.putPixelFct(v_x, v_y, object.color); 
        }
      }
    }
  }
}