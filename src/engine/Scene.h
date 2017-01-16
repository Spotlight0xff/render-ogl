#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include "Model.h"
#include "ShaderCompiler.h"
#include "engine/scene/Camera.h"
#include "engine/Input.h"
#include "engine/scene/FontRender.h"
#include "engine/scene/FpsCounter.h"

#include "engine/scene/SceneObject.h"
//#include "ModelObject.h"
//#include "LightObject.h"

#include "opengl.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <functional>

namespace engine {

//class components::ModelObject;
//class LightObject;

class Scene {
 public:
  Scene(GLFWwindow* w)
  : input(w),
    fps_counter(font, input),
    window(w) { }
  //void addLight(Light const& light);
  //void addModel(Model model, Shader shader) {
  //}

  void addObjectRef(scene::SceneObject* object) {
    objects.push_back(object);
  }

  void setLight(scene::SceneObject* l) {
    objects.push_back(l);
  }

  void useCamera(scene::Camera* cam) { camera = cam; }

  void enableFpsCounter() { enabled_fps = true; }

  void useFont(const char* path) { font.load(path); }


  void draw();

  glm::mat4 getProjectionMatrix() {
    return glm::perspective(45.0f, 1.0f*input.width/input.height, 0.1f, 100.0f);
  }

  Input & getInputRef() { return input; }
  scene::FontRenderer& getFontRef() { return font; }
  scene::Camera const& getCameraRef() const { return *camera; }
  //SceneObject const& getLightRef() const { return *light; }


 private:
  Input input;
  scene::FpsCounter fps_counter;
  scene::Camera* camera;
  GLFWwindow* window;
  scene::FontRenderer font;
  bool enabled_fps = false;
  std::vector<scene::SceneObject*> objects;
  //std::vector<std::reference_wrapper<SceneObject>> components;
};
} // end namespace engine
#endif
