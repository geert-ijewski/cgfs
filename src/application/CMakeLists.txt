add_executable(intro main.cpp)

find_package(OpenGL REQUIRED)

target_link_libraries(
  intro
  PRIVATE OpenGL::GL
          myproject::myproject_options
          myproject::myproject_warnings)

target_link_system_libraries(
  intro
  PRIVATE)

target_include_directories(intro PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
