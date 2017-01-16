//
// Created by spotlight on 1/16/17.
//

#ifndef GRAPHICS_ENGINE_PHONG_H
#define GRAPHICS_ENGINE_PHONG_H

#include "engine/components/Phong/PhongModel.h"
#include "engine/components/Phong/PhongLight.h"

namespace engine {
namespace scene {

class Phong : public SceneObject {
  public:
    Phong()
      : shader_model("phong_model"),
        shader_lights("light_simple") {}
    ~Phong();

    engine::components::PhongModel* addModel(engine::Model& model);
    engine::components::PhongLight* addLight();

    size_t getMaxLights();

    std::vector<engine::components::PhongModel*> getObjects() {
      return objects;
    }

    std::vector<engine::components::PhongLight*> getLights() {
      return lights;
    }

    void draw(Scene& scene);

  private:
    static constexpr size_t max_lights = 4;

    Shader shader_model;
    Shader shader_lights;

    std::vector<engine::components::PhongModel*> objects;
    std::vector<engine::components::PhongLight*> lights;
};

} // end namespace engine::scene
}

#endif //GRAPHICS_ENGINE_PHONG_H
