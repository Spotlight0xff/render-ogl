cmake_minimum_required(VERSION 3.2)
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
find_package(Assimp)
find_package(SOIL REQUIRED)
find_package(Bullet)
pkg_search_module(GLFW REQUIRED glfw3)

add_executable(render ${sources})

target_compile_options(render PUBLIC -std=c++11 -Wall)

target_include_directories(render PUBLIC
  src
  "/usr/include"
  "/usr/include/freetype2"
  "/usr/include/freetype2/freetype"
  ${FREETYPE_INCLUDE_DIRS}
  ${SOIL_INCLUDE_DIR}
  ${BULLET_INCLUDE_DIRS}
  )

# copy recourses into build dir
add_custom_target(copy-runtime-files ALL
    COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
    DEPENDS render)

target_link_libraries(render PUBLIC
  ${OPENGL_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${GLFW_STATIC_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${ASSIMP_LIBRARIES}
  ${SOIL_LIBRARIES}
  ${BULLET_LIBRARIES}
  )

# For the unit tests
find_package(GTest)
if (GTEST_FOUND)
  #add_executable(unit_tests ${sources_test} ${sources})

  # prevent collisions
  #target_compile_definitions(unit_tests PUBLIC UNIT_TESTS)

  # use exec as link library -> inherit compiler options & libs
  #set_target_properties(render PROPERTIES ENABLE_EXPORTS ON)

  #target_link_libraries(unit_tests PUBLIC
    #${GTEST_BOTH_LIBRARIES}
    #render
    #)
endif()
