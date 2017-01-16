#ifndef FONT_RENDER_H
#define FONT_RENDER_H

#include "engine/ShaderCompiler.h"

#include "opengl.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wdocumentation"
#pragma GCC diagnostic ignored "-Wreserved-id-macro"
#pragma GCC diagnostic ignored "-Wpadded"
#include <ft2build.h>
#include FT_FREETYPE_H
#pragma GCC diagnostic pop

#include <unordered_map>
#include <string>

namespace engine {
namespace scene {

struct CharGlyph {
  GLuint texture_id;  // texture id handle
  glm::ivec2 size;    // size of glyph
  glm::ivec2 bearing; // offset from baseline (left top)
  GLuint advance;    // offset to next glyph
};

class FontRenderer {
  public:
    FontRenderer(int window_width, int window_height);

    ~FontRenderer();

    void updateWindow(GLfloat width, GLfloat height);

    bool load(const char *path, FT_UInt height = 48);

    void render(std::string text, GLfloat x, GLfloat y, glm::vec3 color);

  private:
    FT_Library ft;
    Shader shader;
    GLuint vao_font = 0, vbo_font = 0;

    // char -> glyph struct
    std::unordered_map<GLchar, struct CharGlyph> cached_font;

};


} // end namespace engine::scene
}
#endif
