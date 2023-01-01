#include "text.h"

#include <stdio.h>
#include <stdlib.h>

#include "stb_truetype.h"
#include "stb_image.h"

void text_load_ttf(const char *filepath) {
  int x, y, channels;
  unsigned char *buff = stbi_load(filepath, &x, &y, &channels, 4);

  stbtt_fontinfo font_info;
  if (stbtt_InitFont(&font_info, buff, 0)) {
    printf("failed to load font from %s\n", filepath);
  }

  free(buff);
}
