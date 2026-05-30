include(GenerateExportHeader)


add_library(threadpool_library threadpool_library.cpp)


add_library(myproject::threadpool_library ALIAS threadpool_library)

target_link_libraries(threadpool_library PRIVATE myproject_options myproject_warnings)

target_include_directories(threadpool_library ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

if (myproject_BUILD_FUZZ_TESTS)
  target_link_libraries(threadpool_library PRIVATE -fsanitize=fuzzer-no-link)
  target_compile_options(threadpool_library PRIVATE -fsanitize=fuzzer-no-link)
endif()

target_compile_features(threadpool_library PUBLIC cxx_std_${CMAKE_CXX_STANDARD})

set_target_properties(
  threadpool_library
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(threadpool_library EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/myproject/threadpool_library_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(threadpool_library PUBLIC threadpool_library_STATIC_DEFINE)
endif()
