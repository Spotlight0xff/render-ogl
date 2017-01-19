//
// Created by spotlight on 1/16/17.
//

#ifndef GRAPHICS_ENGINE_PHONG_H
#define GRAPHICS_ENGINE_PHONG_H

#include "engine/components/Phong/PhongModel.h"
#include "engine/components/Phong/PhongLight.h"

#include <memory>

namespace engine {
namespace scene {

class Phong : public SceneObject {
  public:

   // Initializes a new scene with Phong lighting
   // This constructor will compile the necessary shaders
    Phong()
      : shader_model("phong_model"),
        shader_lights("light_simple") {}

    // Adds the given model to the scene
    // Returns a smart pointer with the created object
    std::unique_ptr<::engine::components::PhongModel>& addModel(engine::Model& model);

    // Adds a light to the scene.
    // Returns a smart pointer with the created light
    // Note: this function may fail if adding a new light
    // would exceed `max_lights`
    std::unique_ptr<::engine::components::PhongLight>& addLight();

    // updates the shader uniform variables corresponding to
    // the number of active lights
    void updateLights();


    // Returns the maximum number of supported lights
    size_t getMaxLights();

    // Returns a const-ref to the vector containing the models in the scene.
    std::vector<std::unique_ptr<engine::components::PhongModel> > const& getObjects() const {
      return objects;
    }

    // Returns a const-ref to the vector containing the lights in the scene.
    std::vector<std::unique_ptr<engine::components::PhongLight> > const& getLights() const {
      return lights;
    }

    // Draws the PhongScene (so lights and models)
    // in the context of `scene`.
    // This means that this function will for example
    // query the camera position and direction.
    void draw(Scene& scene);

  private:
    // Number of the supported lights
    // Note: You need to synchronize this constant with the corresponding shader
    static constexpr size_t max_lights = 4;

    // Shader instance for the models
    // Includes lighting computations
    ::engine::shader::Compiler shader_model;

    // Shader instance for the light rendering (not the lighting computation)
    ::engine::shader::Compiler shader_lights;

    // Vector managing the objects of the scene
    std::vector<std::unique_ptr<engine::components::PhongModel> > objects;

    // Vector managing the lights of the scene
    std::vector<std::unique_ptr<engine::components::PhongLight> > lights;
};

} // end namespace engine::scene
}

#endif //GRAPHICS_ENGINE_PHONG_H
