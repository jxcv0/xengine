#ifndef TEXT_H_
#define TEXT_H_

#include "lin.h"

struct font_info {
  int bitmap_width;
  int bitmap_height;
  int line_height;
  float scale;
  unsigned char *bitmap;
};

void text_load_ttf(const char* filepath);

void text_debug(vec2 position, const char* str);

#endif  // TEXT_H_
