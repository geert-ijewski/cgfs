#include <cstdint>
#include <myproject/raytracing_library.hpp>


void raytrace(const RaytracingContext& ctx) { 
  const int RED = 255;
  const int GREEN = 255;
  const int BLUE = 255;
  const Color white{ RED, GREEN, BLUE };
  for (int16_t i = 0; i < RED; i++) { ctx.putPixelFct(i, i, white); }
}