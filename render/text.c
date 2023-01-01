#include "text.h"

#include <stdio.h>
#include <stdlib.h>

#include "stb_truetype.h"
#include "utils.h"

void text_load_ttf(const char *filepath) {
  unsigned char *buff = load_file_into_mem_u(filepath);

  stbtt_fontinfo font_info;
  if (stbtt_InitFont(&font_info, buff, 0)) {
    printf("failed to load font from %s\n", filepath);
  }

  int bitmap_width = 512;
  int bitmap_height = 128;
  int line_height = 64;

  unsigned char *bitmap =
      calloc(bitmap_width * bitmap_height, sizeof(unsigned char));

  float scale = stbtt_ScaleForPixelHeight(&font_info, line_height);

  free(buff);
}
