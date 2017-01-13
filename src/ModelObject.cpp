#include "ModelObject.h"
#include "Model.h"
#include "SceneObject.h"
#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>


ModelObject::ModelObject(Model* m, SceneObject const& l, Scene* s)
: model(m),
  shader(),
  light(l),
  scene(s) {

  setShader("resources/shaders/model_phong_v.glsl",
         "resources/shaders/model_phong_f.glsl",
         [](Scene& scene, ModelObject& obj, Shader& shader) {
           glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE,
               glm::value_ptr(obj.getModelMatrix()));
           // Camera view
           glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "view"), 1, GL_FALSE,
               glm::value_ptr(scene.getCameraRef().getViewMatrix()));
           // Projection view
           glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "projection"), 1, GL_FALSE,
               glm::value_ptr(scene.getProjectionMatrix()));


           // Set parameters for shading/etc.
           // Currently using Phong-model
           // TODO: multiple lights
           glUniform3fv(glGetUniformLocation(shader.getId(), "lightColor"), 1, glm::value_ptr(obj.getLightRef().getColor()));
           //glm::vec3 light_pos = glm::vec3(0.0, -7.0, -20.0) + glm::vec3(pos_x, 10.0, pos_z);
           glUniform3fv(glGetUniformLocation(shader.getId(), "lightPos"), 1,
               glm::value_ptr(obj.getLightRef().getPosition()));
           glUniform3fv(glGetUniformLocation(shader.getId(), "cameraPos"), 1,
               glm::value_ptr(scene.getCameraRef().getPosition()));
           // ambient strength is static for now
           glUniform1f(glGetUniformLocation(shader.getId(), "ambientStrength"), 0.1);
         });
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
  glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
  return glm::scale(model, scale);
}


void ModelObject::setShader(const char* shader_vs, const char* shader_fs, ShaderSettingFunc settings) {
  shader = Shader(shader_vs, shader_fs);
  shader_settings = settings;
}

void ModelObject::draw() {
  // use shader program
  shader.use();

  shader_settings(*scene, *this, shader);

  // TODO: remove shader from draw()
  model->draw(shader);
}
