#ifndef MODELOBJECT_H
#define MODELOBJECT_H

#include "engine/Model.h"
#include "engine/shader/compiler.h"
#include "engine/Scene.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

namespace engine {
namespace components {



class ModelObject {// : public engine::SceneObject {
  public:
    ModelObject(Model *m);

    virtual void draw(Scene&) = 0;


    void setPosition(glm::vec3 pos);

    void setScale(glm::vec3 s);

    void setColor(glm::vec3 c) { color = c; }

    glm::vec3 getColor() const { return color; }

    glm::vec3 getPosition() const;

    glm::mat4 getModelMatrix();
    // TODO: move(delta_vec)

  protected:
    glm::vec3 position;
    glm::vec3 scale = glm::vec3(1.0);
    glm::vec3 color;

    std::vector<::engine::shader::Compiler> shaders;


    Model *model;
    ::engine::shader::Compiler shader;
};

} // end namespace engine::components
}
#endif
