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

    void draw(Scene& s) {
      shader.use();
      shader.set("model", getModelMatrix());
      shader.set("view", s.getCameraRef().getViewMatrix());
      shader.set("projection", s.getProjectionMatrix());
      model->draw();
    }

  private:
    ::engine::shader::Compiler shader;
};

} // end namespace engine::components
}
#endif
