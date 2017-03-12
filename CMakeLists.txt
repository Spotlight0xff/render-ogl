cmake_minimum_required(VERSION 3.0)
project(graphics-engine CXX C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")


file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)
file(GLOB_RECURSE SOURCES_TEST tests/*.cpp tests/*.h)

# consume conan settings
message("Binary dir: ${CMAKE_BINARY_DIR}")
if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
	message("Consuming conan buildinfo")
	include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup()
endif()


# Find all required packages
find_package(OpenGL)
find_package(freetype)
find_package(SOIL)
#find_package(Bullet)
#find_package(Threads)
find_package(assimp)
find_package(glew)
find_package(glfw3)


message(glfw3: "${GLFW3_LIBRARIES};${GLFW3_LIBRARIES_STATIC};${GLFW3_STATIC_LIBRARIES};${GLFW3_STATIC}")
message(assimp: "${ASSIMP_LIBRARIES}")
# copy recourses into build dir
add_custom_target(copy-runtime-files ALL
    COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
    DEPENDS sponza)


# compiler and linker options
#set(PROJECT_LIBRARIES
#  ${OPENGL_LIBRARIES}
#  ${GLFW3_LIBRARIES}
#  "C:/Users/spotlight/software/vcpkg/installed/x64-windows-static/lib/glfw3.lib"
#  ${GLEW_LIBRARIES}
#  ${FREETYPE_LIBRARIES}
#  "${ASSIMP_LIBRARIES}.lib"
#  ${SOIL_LIBRARIES}
#  ${BULLET_LIBRARIES}
#  ${THREADS_LIBRARIES}
#  )

link_libraries(${OPENGL_LIBRARIES} assimp-vc140-mtd glew32 glfw3dll ${CONAN_LIBS})

message("${PROJECT_LIBRARIES}")

set(PROJECT_INCLUDE_DIRS
  "src"
  "src/include"
  ${FREETYPE_INCLUDE_DIRS}
  #${SOIL_INCLUDE_DIR}
  #${BULLET_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
  )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

#link_directories(${GLEW_LIBRARY_DIRS} ${ASSIMP_LIBRARY_DIRS})




# compile & link target
add_executable(sponza ${SOURCES} src/demos/sponza.cpp)
#SET_TARGET_PROPERTIES(sponza PROPERTIES
  #INSTALL_RPATH "${INSTALL_RPATH};${GLEW_LIBRARY_DIRS};${ASSIMP_LIBRARY_DIRS}"
  #BUILD_WITH_INSTALL_RPATH TRUE)
target_include_directories(sponza PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries(sponza PUBLIC ${PROJECT_LIBRARIES})


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # for clang++
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    # dont use libc++ for now
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory -stdlib=libc++")
    #target_link_libraries(render PUBLIC c++abi) # use libc++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") # for g++
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel") # for intel c++ compiler
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC") # for visual studio c++
endif()


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
#list(REMOVE_ITEM PROJECT_LIBRARIES   ${GLEW_LIBRARIES})

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


