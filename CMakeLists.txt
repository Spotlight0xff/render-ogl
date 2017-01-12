cmake_minimum_required(VERSION 3.7)
project(render-ogl)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")


file(GLOB_RECURSE sources src/*.cpp src/*.h src/*.vs)
file(GLOB_RECURSE sources_test tests/*.cpp tests/*.h)
file(GLOB_RECURSE data resources/*)

# Find all required packages
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)
find_package(Assimp REQUIRED)
find_package(SOIL REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

add_executable(render ${sources})

target_compile_options(render PUBLIC -std=c++1y -Wall)

target_include_directories(render PUBLIC
  src
  ${FREETYPE_INCLUDE_DIRS}
  ${SOIL_INCLUDE_DIR}
  )

# copy recourses into build dir
file(COPY ${data} DESTINATION resources)
add_custom_target(copy-runtime-files ALL
    COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
    DEPENDS render)

target_link_libraries(render PUBLIC
  ${OPENGL_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${ASSIMP_LIBRARIES}
  ${SOIL_LIBRARIES}
  )

# For the unit tests
find_package(GTest)
if (GTEST_FOUND)
  add_executable(unit_tests ${sources_test} ${sources})

  # prevent collisions
  target_compile_definitions(unit_tests PUBLIC UNIT_TESTS)

  # use exec as link librar -> inherit compiler options & libs
  set_target_properties(render PROPERTIES ENABLE_EXPORTS ON)

  target_link_libraries(unit_tests PUBLIC
    ${GTEST_BOTH_LIBRARIES}
    render
    )
endif()
