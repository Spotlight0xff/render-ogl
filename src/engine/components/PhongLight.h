#ifndef ENGINE_COMPONENTS_PHONGLIGHT_H
#define ENGINE_COMPONENTS_PHONGLIGHT_H

#include "engine/shader/compiler.h"
#include "engine/Scene.h"
#include "engine/model/Model.h"
#include "engine/handler/Camera.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine {
namespace components {

//! @brief Stub class for object which provides light
class PhongLight : public engine::SceneObject {
  public:
    //PhongLight() {}

    //void draw(Scene& scene);

    void drawModel();

    void setPosition(glm::vec3 pos) { position = pos; }

    glm::vec3 getPosition() const { return position; }

    glm::vec3 getColor() const { return glm::vec3(1.0, 1.0, 1.0); }

    glm::mat4 getModelMatrix();

    void renderObject(bool render_object);

  private:
    glm::vec3 position;
    ::engine::model::Model model;

    bool render_object_ = true;

};

}
}
#endif