#ifndef ENGINE_COMPONENTS_PHONGMODEL_H
#define ENGINE_COMPONENTS_PHONGMODEL_H

#include "engine/Model.h"
#include "ModelObject.h"
#include "engine/ShaderCompiler.h"
#include "engine/scene/SceneObject.h"
#include "PhongLight.h"
#include "engine/Scene.h"

#include <glm/gtc/type_ptr.hpp>

namespace engine {
namespace components {

class PhongModel : public ModelObject {
  public:
    PhongModel(Model *m, PhongLight const &l, Scene *s);

  private:
    engine::scene::SceneObject const &light;
};

} // end namespace engine::components
}
#endif
