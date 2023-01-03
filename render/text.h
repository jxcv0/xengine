#ifndef TEXT_H_
#define TEXT_H_

#include <stddef.h>

#include "lin.h"
#include "shader.h"

struct character {
  int m_advance;
  int m_bitmap_width;
  int m_bitmap_height;
  unsigned int m_texture_id;
};

void init_ttf(const char* filepath);

void render_text(const shader_t shader, const mat4 projection, vec2 position,
                 const vec4 color, const char* txt, size_t n);

#endif  // TEXT_H_
