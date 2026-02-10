include(GenerateExportHeader)


add_library(vector_library vector_library.cpp)



add_library(myproject::vector_library ALIAS vector_library)

target_link_libraries(vector_library PRIVATE myproject_options myproject_warnings)

target_include_directories(vector_library ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

if (myproject_BUILD_FUZZ_TESTS)
  target_link_libraries(vector_library PRIVATE -fsanitize=fuzzer-no-link)
  target_compile_options(vector_library PRIVATE -fsanitize=fuzzer-no-link)
endif()

target_compile_features(vector_library PUBLIC cxx_std_${CMAKE_CXX_STANDARD})

set_target_properties(
  vector_library
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(vector_library EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/myproject/vector_library_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(vector_library PUBLIC vector_library_STATIC_DEFINE)
endif()
