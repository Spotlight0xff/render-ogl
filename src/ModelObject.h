#ifndef MODELOBJECT_H
#define MODELOBJECT_H

#include "Model.h"
#include "ShaderCompiler.h"
#include "SceneObject.h"
#include "LightObject.h"
#include "Scene.h"

#include <glm/gtc/type_ptr.hpp>

using ShaderSettingFunc = std::function<void(Scene&, ModelObject&, Shader&)>;

class ModelObject : public SceneObject {
 public:
  ModelObject(Model* m, SceneObject const& l, Scene* s);

  SceneObject const& getLightRef() { return light; }

  void setShader(const char* shader_name, ShaderSettingFunc settings);

  void draw() override;

  Scene& getSceneRef() { return *scene; }


  void setPosition(glm::vec3 pos);
  void setScale(glm::vec3 s);
  void setColor(glm::vec3 c) { color = c; }
  glm::vec3 getColor() const override{ return color; }
  glm::vec3 getPosition() const override;
  glm::mat4 getModelMatrix() override;
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
