#ifndef ENGINE_COMPONENTS_PHONGMODEL_H
#define ENGINE_COMPONENTS_PHONGMODEL_H

#include "engine/Model.h"
#include "engine/components/ModelObject.h"
#include "engine/shader/compiler.h"
#include "PhongLight.h"
#include "engine/Scene.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

namespace engine {
namespace components {

class PhongModel : public ModelObject {
  public:
    PhongModel(Model *m)
    : shader("phong_model"),
      ModelObject(m) {
    }

    void draw(Scene& scene) {
      shader.set("active_lights", 1);

      int i = 0;
      glm::vec3 pos(12.0, 5.0, 5.0);
      std::string plight = "pointLights[" + std::to_string(i) + "].";
      shader.set(plight + "position", pos);
      shader.set(plight + "constant", GLfloat(1.0));
      shader.set(plight + "linear", GLfloat(0.09));
      shader.set(plight + "quadratic", GLfloat(0.032));
      shader.set(plight + "ambient", glm::vec3({0.2f, 0.2f, 0.2f}));
      shader.set(plight + "diffuse", glm::vec3({1.0, 1.0, 1.0}));
      shader.set(plight + "specular", glm::vec3({1.0, 1.0, 1.0}));

      shader.set("projection", scene.getProjectionMatrix());
      shader.set("view", scene.getCameraRef().getViewMatrix());
      shader.set("model", getModelMatrix());
      shader.set("ambientStrength", 0.1f);
      shader.set("viewPos", scene.getCameraRef().getPosition());

      model->draw(shader);
    }

  private:
    ::engine::shader::Compiler shader;
};

} // end namespace engine::components
}
#endif
