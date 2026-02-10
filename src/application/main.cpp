// Minimal Win32 + legacy WGL example that opens a window and renders a
// black frame using the fixed-function pipeline. No external libraries,
// no shaders.
// NOLINTBEGIN(misc-include-cleaner)
#include <windows.h>
#include <chrono>
#include <thread>
#include <GL/gl.h>

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg) {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_CLOSE:
    DestroyWindow(hwnd);
    return 0;
  default:
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
}

int main()
{
  HINSTANCE hInstance = GetModuleHandle(nullptr);

  const char* CLASS_NAME = "SimpleOpenGLWindowClass";

  WNDCLASS wdc{};
  wdc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  wdc.lpfnWndProc = WindowProc;
  wdc.hInstance = hInstance;
  wdc.lpszClassName = CLASS_NAME;

  if (!RegisterClass(&wdc)) {
    return -1;
  }

  const auto DEFAULT_HEIGHT = 800;
  const auto DEFAULT_WIDTH = 600;

  HWND hwnd = CreateWindowEx(
      0,
      CLASS_NAME,
      "Black OpenGL Window",
      WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      DEFAULT_HEIGHT,
      DEFAULT_WIDTH,
      nullptr,
      nullptr,
      hInstance,
      nullptr);

  if (hwnd == nullptr) {
    return -1;
  }

  HDC hdc = GetDC(hwnd);

  const auto BIT_DEPTH_RGB = 24;
  PIXELFORMATDESCRIPTOR pfd{};
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = BIT_DEPTH_RGB;
  pfd.cDepthBits = BIT_DEPTH_RGB;
  pfd.iLayerType = PFD_MAIN_PLANE;

  const int pixelFormat = ChoosePixelFormat(hdc, &pfd);
  if (pixelFormat == 0) {
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);
    return -1;
  }

  if (SetPixelFormat(hdc, pixelFormat, &pfd) != TRUE) {
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);
    return -1;
  }

  HGLRC hglrc = wglCreateContext(hdc);
  if (hglrc == nullptr) {
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);
    return -1;
  }

  if (wglMakeCurrent(hdc, hglrc) != TRUE) {
    wglDeleteContext(hglrc);
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);
    return -1;
  }

  // Set a black clear color and enter the main loop.
  glClearColor(0.0F, 0.0F, 0.0F, 1.0F);

  MSG msg{};
  bool running = true;

  while (running) {
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        running = false;
        break;
      }
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    // Render a single black frame (fixed-function pipeline, no shaders)
    glViewport(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SwapBuffers(hdc);

    // Simple frame limiter (~60 FPS)
    const int FPS_DELIMITER_DELAY = 16; // 1000ms / 60
    std::this_thread::sleep_for(std::chrono::milliseconds(FPS_DELIMITER_DELAY));
  }

  // Cleanup
  wglMakeCurrent(nullptr, nullptr);
  wglDeleteContext(hglrc);
  ReleaseDC(hwnd, hdc);
  DestroyWindow(hwnd);

  UnregisterClass(CLASS_NAME, hInstance);

  return 0;
}
// NOLINTEND(misc-include-cleaner)