#include <cstdint>
#include <myproject/raytracing_library.hpp>


void raytrace(const RaytracingContext& ctx) { 
  const int RED = 255;
  const int GREEN = 255;
  const int BLUE = 255;
  const Color white{ RED, GREEN, BLUE };
  for (int16_t y = 0; y < ctx.height; y++) { // NOLINT(readability-identifier-length)
    for (int16_t x = 0; x < ctx.width; x++) {// NOLINT(readability-identifier-length)
        // paint
        ctx.putPixelFct(x, y, white); 
    }
  }
}