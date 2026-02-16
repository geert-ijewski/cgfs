include(GenerateExportHeader)


add_library(raytracing_library raytracing_library.cpp)



add_library(myproject::raytracing_library ALIAS raytracing_library)

target_link_libraries(raytracing_library PRIVATE myproject_options myproject_warnings)

target_include_directories(raytracing_library ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

if (myproject_BUILD_FUZZ_TESTS)
  target_link_libraries(raytracing_library PRIVATE -fsanitize=fuzzer-no-link)
  target_compile_options(raytracing_library PRIVATE -fsanitize=fuzzer-no-link)
endif()

target_compile_features(raytracing_library PUBLIC cxx_std_${CMAKE_CXX_STANDARD})

set_target_properties(
  raytracing_library
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(raytracing_library EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/myproject/raytracing_library_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(raytracing_library PUBLIC raytracing_library_STATIC_DEFINE)
endif()
