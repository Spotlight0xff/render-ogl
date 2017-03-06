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
            : ModelObject(m) {}

    void drawModel(::engine::shader::Compiler& shader) { model->draw(shader); }

    void draw(Scene&) override {
      model->draw(shader);
    }

  private:
};

} // end namespace engine::components
}
#endif
