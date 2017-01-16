//
// Created by spotlight on 1/16/17.
//

#ifndef GRAPHICS_ENGINE_OPENGL_H_H
#define GRAPHICS_ENGINE_OPENGL_H_H

#ifdef GTEST_TEST
#include "glmock.hpp"
#define APIENTRY
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#endif //GRAPHICS_ENGINE_OPENGL_H_H
