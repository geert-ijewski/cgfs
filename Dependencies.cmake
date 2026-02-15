include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(myproject_setup_dependencies)

  # For each dependency, see if it's
  # already been provided to us by a parent project

  if(NOT TARGET Catch2::Catch2WithMain)
    cpmaddpackage("gh:catchorg/Catch2@3.8.1")
  endif()

CPMAddPackage(
        NAME SDL3
        GITHUB_REPOSITORY libsdl-org/SDL
        GIT_TAG release-3.4.0
        OPTIONS
        "SDL3_DISABLE_INSTALL ON"
        "SDL_SHARED OFF"
        "SDL_STATIC ON"
        "SDL_STATIC_PIC ON"
        "SDL_WERROR OFF"
)
find_package(SDL3 REQUIRED)

file(GLOB SDL3_HEADERS "${SDL3_SOURCE_DIR}/include/*.h")

# Create a target that copies headers at build time, when they change
add_custom_target(sdl_copy_headers_in_build_dir
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${SDL3_SOURCE_DIR}/include" "${CMAKE_BINARY_DIR}/SDLHeaders/SDL3"
        DEPENDS ${SDL3_HEADERS})

# Make SDL depend from it
add_dependencies(SDL3-static sdl_copy_headers_in_build_dir)

# And add the directory where headers have been copied as an interface include dir
target_include_directories(SDL3-static INTERFACE "${CMAKE_BINARY_DIR}/SDLHeaders")

set (SDL3_INCLUDE_DIR ${SDL3_SOURCE_DIR}/include)

include_directories(${SDL3_INCLUDE_DIR})
endfunction()
