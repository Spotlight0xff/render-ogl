#ifndef LIGHTOBJECT_H
#define LIGHTOBJECT_H

#include "SceneObject.h"
#include "ShaderCompiler.h"
#include "Scene.h"
#include "Model.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class LightObject : public SceneObject {
 public:
  LightObject(Scene* s)
  : scene(s),
    model("resources/models/cube.obj"){
    shader = Shader("resources/shaders/light_vertex.glsl", "resources/shaders/light_fragment.glsl");
  }

  void draw() {
    shader.use();
    Camera const& camera = scene->getCameraRef();

    // Model-View-Projection
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE,
        glm::value_ptr(getModelMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "projection"), 1, GL_FALSE,
        glm::value_ptr(scene->getProjectionMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "view"), 1, GL_FALSE,
        glm::value_ptr(camera.getViewMatrix()));
    model.draw(shader);
  }

  void setPosition(glm::vec3 pos) {
    position = pos;
  }

  glm::vec3 getPosition() const override{
    return position;
  }

  glm::vec3 getColor() const override {
    return glm::vec3(1.0, 1.0, 1.0);
  }

  glm::mat4 getModelMatrix() override {
    return glm::translate(glm::mat4(1.0f), position);
  }
 private:
  glm::vec3 position;

  Scene* scene;
  Shader shader;
  Model model;

};
#endif
