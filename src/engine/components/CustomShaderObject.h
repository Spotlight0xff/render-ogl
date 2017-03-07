//
// Created by spotlight on 1/19/17.
//

#ifndef GRAPHICS_ENGINE_CUSTOMSHADEROBJECT_H
#define GRAPHICS_ENGINE_CUSTOMSHADEROBJECT_H

#include "ModelObject.h"
#include "engine/Scene.h"



namespace engine {
namespace components {


class CustomShaderObject : public ModelObject {
    using ShaderSettingFunc = std::function<void(Scene &, ModelObject &, ::engine::shader::Compiler &)>;

  public:
    CustomShaderObject(Model *m, std::string const &shader, ShaderSettingFunc func);

    virtual void draw(Scene&);

  private:
    ShaderSettingFunc settings_;
    ::engine::shader::Compiler shader_;


};

} // end namespace engine::components
}


#endif //GRAPHICS_ENGINE_CUSTOMSHADEROBJECT_H
