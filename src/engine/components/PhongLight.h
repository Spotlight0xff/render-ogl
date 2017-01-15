#ifndef ENGINE_COMPONENTS_PHONGLIGHT_H
#define ENGINE_COMPONENTS_PHONGLIGHT_H

#include "engine/scene/SceneObject.h"
#include "engine/ShaderCompiler.h"
#include "engine/Scene.h"
#include "engine/Model.h"
#include "engine/scene/Camera.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine {
namespace components {

class PhongLight : public engine::scene::SceneObject {
  public:
    PhongLight(Scene *s)
            : scene(s),
              model("resources/models/cube.obj") {
      shader = Shader("light_simple");
    }

    void draw() override;

    void setPosition(glm::vec3 pos) { position = pos; }

    glm::vec3 getPosition() const override { return position; }

    glm::vec3 getColor() const override { return glm::vec3(1.0, 1.0, 1.0); }

    glm::mat4 getModelMatrix() override;

  private:
    glm::vec3 position;

    Scene *scene;
    Shader shader;
    Model model;

};

}
}
#endif
