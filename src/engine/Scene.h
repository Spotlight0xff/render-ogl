#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include "Model.h"
#include "engine/shader/compiler.h"
#include "engine/handler/Camera.h"
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
#include <engine/handler/Camera.h>

namespace engine {

//class components::ModelObject;
//class LightObject;

class Scene {
 public:
  Scene() {}
  //void addLight(Light const& light);
  //void addModel(Model model, Shader shader) {
  //}

  void addObjectRef(::engine::scene::SceneObject* object) {
    objects.push_back(object);
  }

  //void setLight(scene::SceneObject* l) {
  //  objects.push_back(l);
  //}

  void UseCamera(scene::Camera* cam) { camera_ = cam; }

  void enableFpsCounter() { enabled_fps = true; }

  //void useFont(const char* path) { font.load(path); }


  void draw();

  void drawPos();

  glm::mat4 getProjectionMatrix() {
    int width = 1280;
    int height = 1024;
    return glm::perspective(45.0f, 1.0f*width/height, 0.1f, 1000.0f);
  }

  //Input & getInputRef() { return input; }
  //scene::FontRenderer& getFontRef() { return font; }
  scene::Camera const& getCameraRef() const { return *camera_; }
  //SceneObject const& getLightRef() const { return *light; }


 private:
  //Input input;
  //scene::FpsCounter fps_counter;
  scene::Camera* camera_;
  //GLFWwindow* window;
  //scene::FontRenderer font;
  bool enabled_fps = false;
  std::vector<scene::SceneObject*> objects;
  //std::vector<std::reference_wrapper<SceneObject>> components;
};
} // end namespace engine
#endif
