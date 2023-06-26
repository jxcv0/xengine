#include "text.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include "glad.h"
#include "shader.h"

GLuint shader;

struct {
  GLuint tex;
  float size[2];
  float bearing[2];
  unsigned int adv;
} characters[128];
unsigned int VAO, VBO;

/**
 * ----------------------------------------------------------------------------
 */
int init_text(const char *filepath) {
  shader = load_shader_vf("glsl/text.vert", "glsl/text.frag");

  // this is the terrible text rendering from learnopengl.com
  // TODO This would be a lot faster by rendering all glyphs to a single texture
  // instead of one each
  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    return -1;
  }

  FT_Face face;
  if (FT_New_Face(ft, filepath, 0, &face)) {
    return -1;
  }

  FT_Set_Pixel_Sizes(face, 0, 48);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  for (unsigned char c = 0; c < 128; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      continue;
    }
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                 face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    characters[c].tex = tex;
    characters[c].size[0] = face->glyph->bitmap.width;
    characters[c].size[1] = face->glyph->bitmap.rows;
    characters[c].bearing[0] = face->glyph->bitmap_left;
    characters[c].bearing[1] = face->glyph->bitmap_top;
    characters[c].adv = face->glyph->advance.x;
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
void render_text(float x, float y, float scale, char *text, float col[4]) {
  glUseProgram(shader);
  glUniform4f(glGetUniformLocation(shader, "text_color"), col[0], col[1],
              col[2], col[3]);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);

  while (*text) {
    float xpos = x + characters[*text].bearing[0] * scale;
    float ypos =
        y - (characters[*text].size[0] - characters[*text].bearing[1]) * scale;
    float w = characters[*text].size[0] * scale;
    float h = characters[*text].size[1] * scale;

    float vertices[6][4] = {
        {xpos, ypos + h, 0.0f, 0.0f}, {xpos, ypos, 0.0f, 1.0f},
        {xpos + w, ypos, 1.0f, 1.0f}, {xpos, ypos + h, 0.0f, 0.0f},
        {xpos + w, ypos, 1.0f, 1.0f}, {xpos + w, ypos + h, 1.0f, 0.0f}};

    glBindTexture(GL_TEXTURE_2D, characters[*text].tex);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    x += (characters[*text].adv >> 6) * scale;
    ++text;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
