#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <glm/glm.hpp>

class SceneObject {
 public:
  virtual void draw() = 0;
  virtual glm::vec3 getPosition() const = 0;
  virtual glm::mat4 getModelMatrix() = 0;
  virtual glm::vec3 getColor() const = 0;
  virtual ~SceneObject() {}

 private:
  // TODO implement hierarchical structure
  SceneObject* parent = nullptr;
};
#endif
