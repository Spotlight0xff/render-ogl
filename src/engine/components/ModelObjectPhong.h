#ifndef MODELOBJECTPHONG_H
#define MODELOBJECTPHONG_H

#include "engine/Model.h"
#include "ModelObject.h"
#include "engine/ShaderCompiler.h"
#include "engine/scene/SceneObject.h"
#include "LightObject.h"
#include "engine/Scene.h"

#include <glm/gtc/type_ptr.hpp>

namespace engine {
namespace components {

class ModelObjectPhong : public ModelObject {
  public:
    ModelObjectPhong(Model *m, engine::scene::SceneObject const &l, Scene *s);

  private:
    engine::scene::SceneObject const &light;
};

} // end namespace engine::components
}
#endif
