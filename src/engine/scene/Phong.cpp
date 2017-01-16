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
  shader_model.set("projection", scene.getProjectionMatrix());
  shader_model.set("view", scene.getCameraRef().getViewMatrix());

  shader_model.set("lightColor", lights[0]->getColor());
  shader_model.set("lightPos", lights[0]->getPosition());
  shader_model.set("cameraPos", scene.getCameraRef().getPosition());
  shader_model.set("ambientStrength", 0.5f);

  for(auto const& o : objects) {
    shader_model.set("model", o->getModelMatrix());
    o->drawModel(shader_model);
  }

  for(auto const& l: lights) {
    l->draw(scene);
  }
}


} // end namespace engine::scene
}
