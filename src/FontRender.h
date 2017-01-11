#ifndef FONT_RENDER_H
#define FONT_RENDER_H

#include "ShaderCompiler.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wdocumentation"
#pragma GCC diagnostic ignored "-Wreserved-id-macro"
#pragma GCC diagnostic ignored "-Wpadded"
#include <ft2build.h>
#include FT_FREETYPE_H
#pragma GCC diagnostic pop

#include <unordered_map>
#include <string>

struct CharGlyph {
  GLuint texture_id;  // texture id handle
  glm::ivec2 size;    // size of glyph
  glm::ivec2 bearing; // offset from baseline (left top)
  GLuint  advance;    // offset to next glyph
};

class FontRenderer {
 public:
  FontRenderer();
  ~FontRenderer();

  bool load(const char* path, FT_UInt height = 48);
  void render(std::string text, GLfloat x, GLfloat y, glm::vec3 color);

private:
  FT_Library ft;
  Shader shader;
  GLuint vao_font = 0, vbo_font = 0;

  // char -> glyph struct
  std::unordered_map<GLchar, struct CharGlyph> cached_font;
  struct CharGlyph getCached(GLchar character);
  bool isCached(GLchar character);
};


#endif
