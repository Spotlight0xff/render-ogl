#ifndef GRAPHICS_ENGINE_TEXTURE2D_H
#define GRAPHICS_ENGINE_TEXTURE2D_H

#include "engine/Texture.h"
#include "Manager.h"

namespace engine {

class Texture2D : public Texture {
  public:
    enum class Type {
        UNKNOWN,
        DIFFUSE,
        SPECULAR,
    };

    Texture2D(Type type, unsigned char* image, int width, int height, std::string const& path)
            : type_(type),
              path_(path),
              width_(width),
              height_(height),
              shininess_(0.0f) {

        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // Set texture wrapping to GL_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Set texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // TODO: support more colorspaces than RGB
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_,
                     0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0); // unbind
      std::cout << "got texture from path: " << path << "\n";
    }

    std::string getPath() const { return path_; }
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
