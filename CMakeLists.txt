cmake_minimum_required(VERSION 3.0)
project(graphics-engine CXX C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")


file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)
file(GLOB_RECURSE SOURCES_TEST tests/*.cpp tests/*.h)



# Find all required packages
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(SOIL REQUIRED)
find_package(Bullet)
find_package(Threads)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(ASSIMP REQUIRED assimp)
pkg_search_module(GLEW REQUIRED glew)

#

# copy recourses into build dir
add_custom_target(copy-runtime-files ALL
    COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
    DEPENDS render)

# compiler and linker options
set(PROJECT_LIBRARIES
  ${OPENGL_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${GLFW_STATIC_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${ASSIMP_LIBRARIES}
  ${SOIL_LIBRARIES}
  ${BULLET_LIBRARIES}
  ${THREADS_LIBRARIES}
  )

set(PROJECT_INCLUDE_DIRS
  "src"
  "src/include"
  "/usr/include"
  "/usr/include/freetype2"
  "/usr/include/freetype2/freetype"
  ${FREETYPE_INCLUDE_DIRS}
  ${SOIL_INCLUDE_DIR}
  ${BULLET_INCLUDE_DIRS}
  )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

link_directories(${GLEW_LIBRARY_DIRS})


#if(CMAKE_COMPILER_IS_GNUCXX)
    #add_definitions(-std=c++11 -Wall -ansi -Wno-deprecated -pthread)
#endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")
    # using Clang
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    # using GCC
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    # using Intel C++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # using Visual Studio C++
endif()



# compile & link target
add_executable(render ${SOURCES} src/main.cpp)
SET_TARGET_PROPERTIES(render PROPERTIES
  INSTALL_RPATH "${INSTALL_RPATH};${GLEW_LIBRARY_DIRS};${ASSIMP_LIBRARY_DIRS}"
  BUILD_WITH_INSTALL_RPATH TRUE)
target_include_directories(render PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries(render PUBLIC ${PROJECT_LIBRARIES})



###
### Unit testing using Google test
###
set(EXTERNAL_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/external)
set(PROJECT_TEST_NAME unit_tests)
add_subdirectory(${EXTERNAL_PROJECTS_DIR}/gtest)


enable_testing()
add_executable(${PROJECT_TEST_NAME} ${SOURCES_TEST} ${SOURCES})
SET_TARGET_PROPERTIES(${PROJECT_TEST_NAME} PROPERTIES
  INSTALL_RPATH "${INSTALL_RPATH};${GLEW_LIBRARY_DIRS};${ASSIMP_LIBRARY_DIRS}"
  BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
target_include_directories(${PROJECT_TEST_NAME} PUBLIC
  ${PROJECT_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIRS}
  tests
  )

# removing some libraries, we are mocking them :)
list(REMOVE_ITEM PROJECT_LIBRARIES   ${GLEW_LIBRARIES})

# add google test and link that shit
add_dependencies(${PROJECT_TEST_NAME} googletest)
target_link_libraries(${PROJECT_TEST_NAME}
  ${PROJECT_LIBRARIES}
  ${GTEST_LIBRARIES}
  ${GTEST_LIBS_DIR}/libgtest.a
  ${GTEST_LIBS_DIR}/libgtest_main.a
  ${GTEST_LIBS_DIR}/../libgmock.a
  ${GTEST_LIBS_DIR}/../libgmock_main.a
  )
target_link_libraries(${PROJECT_TEST_NAME} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME ${PROJECT_TEST_NAME} COMMAND ${PROJECT_TEST_NAME})


