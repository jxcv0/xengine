#ifndef TEXT_H_
#define TEXT_H_

#include "lin.h"

struct character {
  int m_advance;
  int m_codepoint;
  int m_bitmap_width;
  int m_bitmap_height;
  int m_xoff;
  int m_yoff;
  unsigned int m_texture_id;
  unsigned char* mp_bitmap;
};

void init_ttf(const char* filepath);

void text_debug(vec2 position, const char* txt);

void clean_up(void);

#endif  // TEXT_H_
