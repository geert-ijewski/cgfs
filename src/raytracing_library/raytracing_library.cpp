#include <cstdint>
#include <myproject/raytracing_library.hpp>


void raytrace(const RaytracingContext& ctx) { 
  for (int16_t y = 0; y < ctx.height; y++) { // NOLINT(readability-identifier-length)
    for (int16_t x = 0; x < ctx.width; x++) {// NOLINT(readability-identifier-length)
      for (const auto& object : ctx.objects) { 
        if (object.pos.x == x && object.pos.y == y) { 
            ctx.putPixelFct(x, y, object.color); 
        }
      }
    }
  }
}