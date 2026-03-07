add_executable(application main.cpp)

find_package(OpenGL REQUIRED)

target_link_libraries(
  application
  PRIVATE OpenGL::GL
          SDL3::SDL3
          raytracing_library
          myproject::myproject_options
          myproject::myproject_warnings)

target_link_system_libraries(
  application
  PRIVATE)

target_include_directories(application PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")

if(EMSCRIPTEN)
  myproject_configure_wasm_target(intro
    TITLE "Raytracer"
    DESCRIPTION "Raytracer with fixed scene"
  )
endif()
