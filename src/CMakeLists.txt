add_subdirectory(vector_library)

if (WIN32)
  add_subdirectory(windows)
elseif (UNIX)
  add_subdirectory(unix)
else()
  message(STATUS "Platform ${CMAKE_SYSTEM_NAME} is not explicitly supported; skipping windows/unix subdirectories.")
endif()
