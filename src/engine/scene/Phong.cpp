//
// Created by spotlight on 1/16/17.
//

#include "Phong.h"

namespace engine {
namespace scene {

Phong::~Phong() {
  for(auto& o : objects) {
    delete o;
  }
  for(auto& l : lights) {
    delete l;
  }
}

engine::components::PhongModel* Phong::addModel(engine::Model& model) {
  using ::engine::components::PhongModel;
  PhongModel* obj = new PhongModel(&model);
  objects.push_back(obj);
  return obj;
}

engine::components::PhongLight* Phong::addLight() {
  using ::engine::components::PhongLight;
  if (lights.size() >= max_lights) {
    return nullptr;
  }
  PhongLight* light = new PhongLight();
  lights.push_back(light);
  return light;
}

size_t Phong::getMaxLights() {
  return max_lights;
}

void Phong::draw(Scene& scene) {
  /*shader_lights.set("projection", scene.getProjectionMatrix());
  shader_lights.set("view", scene.getCameraRef().getViewMatrix());
  */
  int i = 0;
  shader_model.set("active_lights", GLint(lights.size()));

  for(auto const& l: lights) {
    std::string plight = "pointLights[" + std::to_string(i) + "].";
    shader_model.set(plight + "position", l->getPosition());
    shader_model.set(plight + "constant", GLfloat(1.0));
    shader_model.set(plight + "linear", GLfloat(0.09));
    shader_model.set(plight + "quadratic", GLfloat(0.032));
    shader_model.set(plight + "ambient", {0.05f, 0.05f, 0.05f});
    shader_model.set(plight + "diffuse", {1.0, 1.0, 1.0});
    shader_model.set(plight + "specular", {1.0, 1.0, 1.0});

    shader_lights.set("projection", scene.getProjectionMatrix());
    shader_lights.set("view", scene.getCameraRef().getViewMatrix());
    shader_lights.set("model", l->getModelMatrix());
    l->drawModel();
    i ++;
  }



  for(auto const& o : objects) {
    shader_model.set("projection", scene.getProjectionMatrix());
    shader_model.set("view", scene.getCameraRef().getViewMatrix());
    shader_model.set("model", o->getModelMatrix());
    shader_model.set("ambientStrength", 0.1f);
    shader_model.set("viewPos", scene.getCameraRef().getPosition());

    o->drawModel(shader_model);
  }
}


} // end namespace engine::scene
}
