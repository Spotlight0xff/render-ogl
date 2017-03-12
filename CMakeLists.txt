cmake_minimum_required(VERSION 3.0)
project(graphics-engine CXX C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

set(SOURCES src/engine/Timing.cpp src/engine/handler/FpsMovement.cpp src/engine/handler/EulerCamera.cpp src/engine/particles/RandomTexture.cpp src/engine/particles/ShaderUpdater.cpp src/engine/particles/System.cpp src/engine/Manager.cpp src/engine/components/PhongLight.cpp src/engine/components/CustomShaderObject.cpp src/engine/components/ModelObject.cpp src/engine/Engine.cpp src/engine/model/Model.cpp src/engine/model/Mesh.cpp src/engine/Scene.cpp src/engine/Texture2D.cpp src/engine/shader/compiler.cpp src/Util.cpp)
set(SOURCES_TEST tests/performance/render.cpp tests/util/split.cpp tests/engine/Model.cpp tests/engine/Shader.cpp tests/glmock.cpp)

macro(use_package pkg)
	find_package(${pkg})
	string(TOUPPER "${pkg}" upper_pkg)
	if(${${upper_pkg}_FOUND})
		# default find_package succeeded
		list(APPEND PROJECT_INCLUDE_DIRS "${${upper_pkg}_INCLUDE_DIRS}")
		list(APPEND PROJECT_LIBRARIES "${${upper_pkg}_LIBRARIES}")
	else()
		message("Not found: ${pkg}, trying with Conan")
		# find_package failed, maybe we're building with conan
		# TODO: more extensive search with conan (and individual linking)
		list(APPEND  PROJECT_LIBRARIES ${CONAN_LIBS})
	endif()
endmacro()


# consume conan packages
if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
	include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup()
endif()


# Find all required packages
# if needed, import from conan
use_package(OpenGL)
use_package(Freetype)
use_package(SOIL)
use_package(GTest)
use_package(GMock)
use_package(GLEW)
use_package(GLFW)
use_package(assimp)
use_package(Threads)


# copy recourses into build dir
add_custom_target(copy-runtime-files ALL
    COMMAND cmake -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
    DEPENDS sponza)

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




# compile & link target
add_executable(sponza ${SOURCES} src/demos/sponza.cpp)
SET_TARGET_PROPERTIES(sponza PROPERTIES
  INSTALL_RPATH "${INSTALL_RPATH};${GLEW_LIBRARY_DIRS};${ASSIMP_LIBRARY_DIRS}"
  BUILD_WITH_INSTALL_RPATH TRUE)
target_include_directories(sponza PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries(sponza PUBLIC ${PROJECT_LIBRARIES})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # for clang++
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    # dont use libc++ for now
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") # for g++
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel") # for intel c++ compiler
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC") # for visual studio c++
endif()


###
### Unit testing using Google test
###
set(PROJECT_TEST_NAME unit_tests)

enable_testing()
add_executable(${PROJECT_TEST_NAME} ${SOURCES_TEST} ${SOURCES})
SET_TARGET_PROPERTIES(${PROJECT_TEST_NAME} PROPERTIES
  INSTALL_RPATH "${INSTALL_RPATH};${GLEW_LIBRARY_DIRS};${ASSIMP_LIBRARY_DIRS}"
  BUILD_WITH_INSTALL_RPATH TRUE)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
target_include_directories(${PROJECT_TEST_NAME} PUBLIC
  ${PROJECT_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIRS}
  tests
  )
target_compile_definitions(${PROJECT_TEST_NAME} PRIVATE UNIT_TESTING=1)

# removing some libraries, we are mocking them :)
list(REMOVE_ITEM PROJECT_LIBRARIES   ${GLEW_LIBRARIES})

# add google test and link that shit
target_link_libraries(${PROJECT_TEST_NAME}
  ${PROJECT_LIBRARIES}
  ${GMOCK_BOTH_LIBRARIES}
  ${GTEST_BOTH_LIBRARIES}
  )
target_link_libraries(${PROJECT_TEST_NAME} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME ${PROJECT_TEST_NAME} COMMAND ${PROJECT_TEST_NAME})
