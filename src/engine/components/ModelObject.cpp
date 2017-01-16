#include "ModelObject.h"
#include "engine/scene/SceneObject.h"
#include "engine/Scene.h"

#include <glm/gtc/matrix_transform.hpp>


namespace engine {
namespace components {

ModelObject::ModelObject(Model *m)
        : model(m),
          shader(){
}

void ModelObject::setPosition(glm::vec3 pos) {
  position = pos;
}

void ModelObject::setScale(glm::vec3 s) {
  scale = s;
}

glm::vec3 ModelObject::getPosition() const {
  return position;
}

glm::mat4 ModelObject::getModelMatrix() {
  glm::mat4 mat_model = glm::translate(glm::mat4(1.0f), position);
  return glm::scale(mat_model, scale);
}


void ModelObject::setShader(const char *shader_name, ShaderSettingFunc settings) {
  shader = Shader(shader_name);
  shader_settings = settings;
}

void ModelObject::draw(Scene& scene) {
  // use shader program
  shader.use();

  shader_settings(scene, *this, shader);

  // TODO: remove shader from draw()
  model->draw(shader);
}

}  // namespace engine::components
}
