#ifndef MODELOBJECT_H
#define MODELOBJECT_H

#include "engine/Model.h"
#include "engine/ShaderCompiler.h"
#include "engine/scene/SceneObject.h"
#include "engine/Scene.h"

#include <glm/gtc/type_ptr.hpp>

namespace engine {
namespace components {



class ModelObject : public engine::scene::SceneObject {
    using ShaderSettingFunc = std::function<void(Scene &, ModelObject &, Shader &)>;
  public:
    ModelObject(Model *m, Scene *s);

    void setShader(const char *shader_name, ShaderSettingFunc settings);

    void draw() override;

    Scene &getSceneRef() { return *scene; }


    void setPosition(glm::vec3 pos);

    void setScale(glm::vec3 s);

    void setColor(glm::vec3 c) { color = c; }

    glm::vec3 getColor() const override { return color; }

    glm::vec3 getPosition() const override;

    glm::mat4 getModelMatrix() override;
    // TODO: move(delta_vec)

  protected:
    glm::vec3 position;
    glm::vec3 scale = glm::vec3(1.0);
    glm::vec3 color;

    std::vector<Shader> shaders;


    Model *model;
    Shader shader;
    ShaderSettingFunc shader_settings = [](Scene &s, ModelObject &m, Shader &sh) {};
    Scene *scene;
};

} // end namespace engine::components
}
#endif
