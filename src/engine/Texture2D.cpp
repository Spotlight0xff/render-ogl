//
// Created by spotlight on 3/10/17.
//

#include "Texture2D.h"

namespace engine {

Texture2D::Texture2D(Texture2D::Type type, unsigned char *image, int width, int height, std::string const &path)
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
}

} // end namespace engine
