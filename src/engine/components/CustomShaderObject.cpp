//
// Created by spotlight on 1/19/17.
//

#include "CustomShaderObject.h"

namespace engine {
namespace components {


CustomShaderObject::CustomShaderObject(Model* m, std::string const& shader, ShaderSettingFunc func)
        : shader_(shader),
          settings_(func),
          ModelObject(m) {
}

void CustomShaderObject::draw(Scene& scene) {
  shader_.use();
  settings_(scene, *this, shader_);
  model->draw();
}

}
}