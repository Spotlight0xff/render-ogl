//
// Created by spotlight on 1/19/17.
//

#ifndef GRAPHICS_ENGINE_CUSTOMSHADEROBJECT_H
#define GRAPHICS_ENGINE_CUSTOMSHADEROBJECT_H

#include "ModelObject.h"
#include "engine/Scene.h"



namespace engine {
namespace components {


/*!
 * Object in a scene using a custom shader.
 *
 * This object inherits from ModelObject, so it is created from a `Model`.
 *
 */
class CustomShaderObject : public ModelObject {
    using ShaderSettingFunc = std::function<void(Scene &, ModelObject &, ::engine::shader::Compiler &)>;

  public:
    /*!
     * Constructor from a given model and shader and its settings.
     *
     * @param m model from which to construct
     * @param shader shadername
     * @param func function to set shader settings
     */
    CustomShaderObject(Model *m, std::string const &shader, ShaderSettingFunc func);

    virtual void draw(Scene&);

  private:
    //! Holds shader settings (as function)
    ShaderSettingFunc settings_;

    //! compiled shader object
    ::engine::shader::Compiler shader_;
};

} // end namespace engine::components
}


#endif //GRAPHICS_ENGINE_CUSTOMSHADEROBJECT_H
