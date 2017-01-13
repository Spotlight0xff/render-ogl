#ifndef SCENE_H
#define SCENE_H

#include "Model.h"
#include "ShaderCompiler.h"
#include "Camera.h"
#include "Input.h"
#include "FontRender.h"
#include "FpsCounter.h"

#include "SceneObject.h"
//#include "ModelObject.h"
//#include "LightObject.h"

// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <functional>

class ModelObject;
class LightObject;

class Scene {
 public:
  Scene(GLFWwindow* w)
  : input(w),
    fps_counter(font, input),
    window(w) { }
  //void addLight(Light const& light);
  //void addModel(Model model, Shader shader) {
  //}

  void addObjectRef(SceneObject* object) {
    objects.push_back(object);
  }

  void setLight(SceneObject* l) {
    objects.push_back(l);
  }

  void useCamera(Camera* cam) { camera = cam; }

  void enableFpsCounter() { enabled_fps = true; }

  void useFont(const char* path) { font.load(path); }


  void draw();

  glm::mat4 getProjectionMatrix() {
    return glm::perspective(45.0f, 1.0f*input.width/input.height, 0.1f, 100.0f);
  }

  Input & getInputRef() { return input; }
  FontRenderer& getFontRef() { return font; }
  Camera const& getCameraRef() const { return *camera; }
  //SceneObject const& getLightRef() const { return *light; }


 private:
  Input input;
  FpsCounter fps_counter;
  Camera* camera;
  GLFWwindow* window;
  FontRenderer font;
  bool enabled_fps = false;
  std::vector<SceneObject*> objects;
  //std::vector<std::reference_wrapper<SceneObject>> objects;
};
#endif
