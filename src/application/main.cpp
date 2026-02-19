//NOLINTBEGIN(misc-include-cleaner,cppcoreguidelines-pro-type-vararg,hicpp-vararg)
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */ //NOLINT(cppcoreguidelines-macro-usage)
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <myproject/raytracing_library.hpp>


/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = nullptr; //NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
static SDL_Renderer *renderer = nullptr;// NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
static Uint64 last_time = 0;// NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;


/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
  (void)appstate;
  (void)argc;
  (void)argv;

  SDL_SetAppMetadata("Computer Graphics From Scratch", "1.0", "com.example.cgfs");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer(
        "examples/renderer/points", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

  last_time = SDL_GetTicks();

  const auto& raytracingCtx = new RaytracingContext();
  raytracingCtx->putPixelFct = [](int16_t x, int16_t y, Color color) { //NOLINT(bugprone-easily-swappable-parameters)
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderPoint(renderer, x, y);
  };

  const int POS = 100;
  const int COLOR = 128;
  raytracingCtx->objects.emplace_back(Vector3d(POS, POS, 0), Color(COLOR, COLOR, 0));
  raytracingCtx->height = WINDOW_HEIGHT;
  raytracingCtx->width = WINDOW_WIDTH;
  *appstate = (void *)raytracingCtx;// cppcheck-suppress[cstyleCast]

  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) // cppcheck-suppress[constParameterCallback]
{
  (void)appstate;
  if (event->type == SDL_EVENT_QUIT) { return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */ }
  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
  (void)appstate;
  const Uint64 now = SDL_GetTicks();
  const float elapsed = ((float)(now - last_time)) / 1000.0F; /* seconds since last iteration */
  SDL_Log("Seconds since last frame: %f", elapsed);

  last_time = now;

  /* as you can see from this, rendering draws over whatever was drawn before it. */
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); /* black, full alpha */
  SDL_RenderClear(renderer); /* start with a blank canvas. */

  raytrace(*reinterpret_cast<RaytracingContext *>(appstate)); //NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)

  SDL_RenderPresent(renderer); /* put it all on the screen! */

  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) { 
  (void)appstate;
  (void)result;
  /* SDL will clean up the window/renderer for us. */
}
// NOLINTEND(misc-include-cleaner,cppcoreguidelines-pro-type-vararg,hicpp-vararg)