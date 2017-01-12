#include "FontRender.h"
#include "ShaderCompiler.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

FontRenderer::FontRenderer()
{
  if(FT_Init_FreeType(&ft)) {
    std::cerr << "Failed to initialize freetype2 library\n";
    return;
  }
  shader = Shader ("resources/shaders/font_vertex.glsl", "resources/shaders/font_fragment.glsl");

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 1024

  glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(WINDOW_WIDTH), 0.0f, static_cast<GLfloat>(WINDOW_HEIGHT));
  shader.use();
  glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));


  // Text rendering setup for OpenGL
  glGenVertexArrays(1, &vao_font);
  glGenBuffers(1, &vbo_font);

  glBindVertexArray(vao_font);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_font);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4* sizeof(GLfloat), 0);
  glBindVertexArray(0);
  return;
}

FontRenderer::~FontRenderer() {
  glDeleteBuffers(1, &vbo_font);
  glDeleteVertexArrays(1, &vao_font);
  FT_Done_FreeType(ft);
}


bool FontRenderer::isCached(GLchar character) {
  auto it_char = cached_font.find(character);
  if (it_char != cached_font.end()) {
    return true;
  }
  return false;
}

bool FontRenderer::load(const char* path, FT_UInt height) {
  FT_Face face;
  if (FT_New_Face(ft, path, 0, &face)) {
    std::cerr << "Failed to load font " << path << std::endl;
    return false;
  }
  FT_Set_Pixel_Sizes(face, 0, height);

  std::cout << "load " << path << std::endl;

  // disable byte-alignment restriction
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // cache ASCII table
  for (GLubyte c = 0; c < 128; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cerr << "Error:Freetype: Failed to load character '"<< c << "'" << std::endl;
      continue; // pray
    }

    GLuint tex_id;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED, // red is first byte of color vector
        static_cast<GLsizei>(face->glyph->bitmap.width),
        static_cast<GLsizei>(face->glyph->bitmap.rows),
        0,
        GL_RED,
        GL_UNSIGNED_BYTE, // TODO for unicode?
        face->glyph->bitmap.buffer
        );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    CharGlyph glyph = {
      tex_id,
      glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
      glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
      static_cast<GLuint>(face->glyph->advance.x),
    };
    // set glyph stuff
    cached_font.insert(std::pair<GLchar, struct CharGlyph>(c, glyph));
  }
  std::string font(path);
  FT_Done_Face(face);
  return true;
}

void FontRenderer::render(std::string text, GLfloat x, GLfloat y, glm::vec3 color) {
  GLfloat scale = 1.0f;
  // use shader
  shader.use();
  glUniform3f(glGetUniformLocation(shader.getId(), "textColor"),
      color.x, color.y, color.z);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(vao_font);
  for (auto const& c : text) {
    struct CharGlyph glyph = cached_font[c];
    GLfloat x_pos = x + glyph.bearing.x * scale;
    GLfloat y_pos = y - (glyph.size.y - glyph.bearing.y) * scale;

    GLfloat w = glyph.size.x * scale;
    GLfloat h = glyph.size.y * scale;

    // Update VBO
    GLfloat vertices[6][4] = {
      { x_pos,     y_pos + h,   0.0, 0.0 },            
      { x_pos,     y_pos,       0.0, 1.0 },
      { x_pos + w, y_pos,       1.0, 1.0 },

      { x_pos,     y_pos + h,   0.0, 0.0 },
      { x_pos + w, y_pos,       1.0, 1.0 },
      { x_pos + w, y_pos + h,   1.0, 0.0 }   
    };
    // render glyph texture
    glBindTexture(GL_TEXTURE_2D, glyph.texture_id);

    // copy that shit onto the GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo_font);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    x += (glyph.advance >> 6) * scale;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
