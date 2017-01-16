#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>


namespace engine {

class Scene;

namespace scene {


class SceneObject {
  public:
    virtual void draw(::engine::Scene& scene) = 0;

    //virtual glm::vec3 getPosition() const = 0;

    //virtual glm::mat4 getModelMatrix() = 0;

    //virtual glm::vec3 getColor() const = 0;

    virtual ~SceneObject() {}

  private:
    // TODO implement hierarchical structure
    SceneObject *parent = nullptr;
};

}
}
#endif
