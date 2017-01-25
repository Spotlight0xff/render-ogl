#include "Phong.h"

#include <memory>

namespace engine {
namespace scene {

// Dynamically allocates a PhongModel,
// then transfers the ownership to the objects-vector
// Returns a const reference to the created model
std::unique_ptr<::engine::components::PhongModel>& Phong::addModel(engine::Model& model) {
  using ::engine::components::PhongModel;
  std::unique_ptr<PhongModel> obj(new PhongModel(&model));
  objects.push_back(std::move(obj));
  return objects.back();
}

// Dynamically allocates a PhongLight,
// then transfers the ownership to the objects-vector.
// Returns a const reference to the created light object
// Note: This function may fail if the number of lights exceed `max_lights`.
//       In this case, addLight() will return the reference to a nullptr
std::unique_ptr<engine::components::PhongLight>& Phong::addLight() {
  using ::engine::components::PhongLight;
  // hmm, ugly. Maybe use boost::optional?
  static std::unique_ptr<PhongLight> not_found(nullptr);
  if (lights.size() >= max_lights) {
    return not_found;
  }
  std::unique_ptr<PhongLight> light(new PhongLight());
  lights.push_back(std::move(light));
  updateLights();
  return lights.back();
}

// update the shader uniform variable for active lights
// Should get called every time the number of lights is changed!
inline void Phong::updateLights() {
  shader_model.set("active_lights", GLint(lights.size()));
}

inline size_t Phong::getMaxLights() {
  return max_lights;
}

// Draws the phong scene in the context of `scene`.
// The two loops will iterate over the lights and objects
// and set the needed shader variables.
// It will query `scene` for:
//  * camera viewmatrix
//  * camera position
//  * projection matrix
//
// possible optimizations:
// * set uniform variables for the corresponding shaders in advance
// * the the matrices in front and then just use the refs
void Phong::draw(Scene& scene) {
  /*shader_lights.set("projection", scene.getProjectionMatrix());
  shader_lights.set("view", scene.getCameraRef().getViewMatrix());
  */
  int i = 0;

  for(auto const& l: lights) {
    std::string plight = "pointLights[" + std::to_string(i) + "].";
    shader_model.set(plight + "position", l->getPosition());
    shader_model.set(plight + "constant", GLfloat(0.1));
    shader_model.set(plight + "linear", GLfloat(0.05));
    shader_model.set(plight + "quadratic", GLfloat(0.01));
    shader_model.set(plight + "ambient", glm::vec3({0.7f, 0.7f, 0.7f}));
    shader_model.set(plight + "diffuse", glm::vec3({1.0, 1.0, 1.0}));
    shader_model.set(plight + "specular", glm::vec3({1.0, 1.0, 1.0}));

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
