add_executable(unix_application main.cpp)

find_package(OpenGL REQUIRED)

target_link_libraries(
  unix_application
  PRIVATE OpenGL::GL
          myproject::myproject_options
          myproject::myproject_warnings)

target_link_system_libraries(
  unix_application
  PRIVATE)

target_include_directories(unix_application PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
