#include "ModelObjectPhong.h"
#include "engine/Model.h"
#include "engine/scene/SceneObject.h"
#include "engine/Scene.h"

#include <glm/gtc/matrix_transform.hpp>

namespace engine {
namespace components {


ModelObjectPhong::ModelObjectPhong(Model *m, engine::scene::SceneObject const &l, Scene *s)
        : ModelObject(m, s),
          light(l) {
  setShader("phong_model",
            [this](Scene &scene, ModelObject &obj, Shader &shader) {
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
              glUniform3fv(glGetUniformLocation(shader.getId(), "lightColor"), 1,
                           glm::value_ptr(this->light.getColor()));
              //glm::vec3 light_pos = glm::vec3(0.0, -7.0, -20.0) + glm::vec3(pos_x, 10.0, pos_z);
              glUniform3fv(glGetUniformLocation(shader.getId(), "lightPos"), 1,
                           glm::value_ptr(this->light.getPosition()));
              glUniform3fv(glGetUniformLocation(shader.getId(), "cameraPos"), 1,
                           glm::value_ptr(scene.getCameraRef().getPosition()));
              // ambient strength is static for now
              glUniform1f(glGetUniformLocation(shader.getId(), "ambientStrength"), 0.5);
            });
}

} // end namespace engine::components
}
