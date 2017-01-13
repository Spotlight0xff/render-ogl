#ifndef MODELOBJECT_H
#define MODELOBJECT_H

#include "Model.h"
#include "ShaderCompiler.h"
#include "SceneObject.h"
#include "LightObject.h"
#include "Scene.h"

#include <glm/gtc/type_ptr.hpp>


class ModelObject : public SceneObject {
 public:
  ModelObject(Model* m, SceneObject const& l, Scene* s)
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

  SceneObject const& getLightRef() { return light; }

  void setShader(const char* shader_vs, const char* shader_fs, std::function<void(Scene&, ModelObject&, Shader&)> settings) {
    shader = Shader(shader_vs, shader_fs);
    shader_settings = settings;
  }

  ~ModelObject() {
  }

  void draw() {
    // use shader program
    shader.use();

    shader_settings(*scene, *this, shader);

    // TODO: remove shader from draw()
    model->draw(shader);


    // Model-View-Project matrices
    // Model

    //model->draw(shader);

  }

  Scene& getSceneRef() { return *scene; }


  void setPosition(glm::vec3 pos);
  void setScale(glm::vec3 s);
  void setColor(glm::vec3 c) {
    color = c;
  }
  glm::vec3 getColor() const override{
    return color;
  }
  glm::vec3 getPosition() const override;
  glm::mat4 getModelMatrix();
  // TODO: move(delta_vec)

 private:
  glm::vec3 position;
  glm::vec3 scale = glm::vec3(1.0);
  glm::vec3 color;

  std::vector<Shader> shaders;


  Model* model;
  Shader shader;
  std::function<void(Scene&, ModelObject&, Shader&)> shader_settings;
  SceneObject const& light;
  Scene* scene;
};
#endif
