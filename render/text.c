#include "text.h"

#include <stdio.h>
#include <stdlib.h>

#include "glad.h"
#include "stb_truetype.h"
#include "utils.h"

float scale;
struct character debug_chars[128];
unsigned int vao;
unsigned int vbo;

/**
 * ----------------------------------------------------------------------------
 */
void init_ttf(const char *filepath) {
  unsigned char *tff_buffer = load_file_into_mem_u(filepath);
  if (tff_buffer == NULL) {
    fprintf(stderr, "unable to load ttf file from %s\n", filepath);
    return;
  }

  stbtt_fontinfo info;
  if (stbtt_InitFont(&info, tff_buffer, 0)) {
    int ascent, descent, line_gap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &line_gap);
    scale = stbtt_ScaleForPixelHeight(&info, 64);

    // first 128 utf8 codes
    for (int i = 0; i < 128; i++) {
      // TODO this can be made a lot faster by allocating temp buffer manually
      // with dimensions from functions below.
      // stbtt_GetCodepointBitmapBox(&info, i, 1, 1, &x0, &y0, &x1, &y1);
      // stbtt_MakeCodepointBitmap(&info, bmp_buff, x, y, adv_width,

      // get character width
      int adv_width;
      int left_side_bearing;
      stbtt_GetCodepointHMetrics(&info, i, &adv_width, &left_side_bearing);

      // set up character with bitmap and dimensions
      struct character c = {0};
      c.m_codepoint = i;
      c.mp_bitmap =
          stbtt_GetCodepointBitmap(&info, 1, 1, i, &c.m_bitmap_width,
                                   &c.m_bitmap_height, &c.m_xoff, &c.m_yoff);

      // generate OpenGL texture
      glGenTextures(1, &c.m_texture_id);
      glBindTexture(GL_TEXTURE_2D, c.m_texture_id);

      // using red channel
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, c.m_bitmap_width,
                   c.m_bitmap_height, 0, GL_RED, GL_UNSIGNED_BYTE, c.mp_bitmap);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      debug_chars[i] = c;
    }
  } else {
    fprintf(stderr, "unable to load ttf file from %s\n", filepath);
  }

  free(tff_buffer);

  // gen quad for rendering characters
  glGenBuffers(1, &vbo);
  glGenVertexArrays(1, &vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindVertexArray(vao);

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
}

/**
 * ----------------------------------------------------------------------------
 */
void render_text(const shader_t shader, const mat4 projection, vec2 position,
                 const vec4 color, const char *txt, size_t n) {
  glUseProgram(shader);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(0);

  shader_set_uniform_m4fv(shader, "projection", projection);
  shader_set_uniform_4fv(shader, "text_color", color);

  for (size_t i = 0; i < n; i++) {
    int code = txt[i];
    struct character c = debug_chars[code];
    float xpos = position[0];
    float ypos = position[1];
    float w = (float)c.m_bitmap_width * scale;
    float h = (float)c.m_bitmap_height * scale;
    printf("%f %d\n", w, c.m_bitmap_width);

    float vertices[6][4] = {
        {xpos, ypos + h, 0.0f, 0.0f}, {xpos, ypos, 0.0f, 1.0f},
        {xpos + w, ypos, 1.0f, 1.0f}, {xpos, ypos + h, 0.0f, 0.0f},
        {xpos + w, ypos, 1.0f, 1.0f}, {xpos + w, ypos + h, 1.0f, 0.0f}};

    glBindTexture(GL_TEXTURE_2D, c.m_texture_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    position[0] += (float)c.m_advance * scale;
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void clean_up(void) {
  for (int i = 0; i < 128; i++) {
    stbtt_FreeBitmap(debug_chars[i].mp_bitmap, NULL);
  }
}
