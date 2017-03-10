#ifndef GRAPHICS_ENGINE_TEXTURE2D_H
#define GRAPHICS_ENGINE_TEXTURE2D_H

#include "engine/Texture.h"
#include "Manager.h"

namespace engine {

//! @brief 2D texture from file
class Texture2D : public Texture {
  public:
    enum class Type {
        UNKNOWN,
        DIFFUSE,
        SPECULAR,
    };

    /*!
     * Constructs a new 2D texture from raw image data.
     *
     * @param type texture type
     * @param image raw image data
     * @param width width of texture
     * @param height height of texture
     * @param path path to texture
     */
    Texture2D(Type type, unsigned char* image, int width, int height, std::string const& path);

    //! Getter of `path_`.
    //! \return path of texture, of empty if none was provided
    std::string getPath() const { return path_; }

    //! Getter of `type_`.
    //! \return texture type
    Type getType() const { return type_; }

    float shininess_ = 1.0;
    glm::vec3 diffuse_ = glm::vec3(0.5f);
    glm::vec3 ambient_ = glm::vec3(0.1f);
    glm::vec3 specular_ = glm::vec3(1.0f);

  private:
    Type type_;
    std::string path_;
    int width_;
    int height_;
};


} // end namespace engine
#endif //GRAPHICS_ENGINE_TEXTURE2D_H
