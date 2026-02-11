add_executable(windows_application main.cpp)

find_package(OpenGL REQUIRED)

target_link_libraries(
  windows_application
  PRIVATE OpenGL::GL
          myproject::myproject_options
          myproject::myproject_warnings)

target_link_system_libraries(
  windows_application
  PRIVATE)

target_include_directories(windows_application PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
