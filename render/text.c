#include "text.h"

#include <stdio.h>
#include <stdlib.h>

#include "stb_truetype.h"
#include "utils.h"

void init_ttf(const char *filepath) {
  unsigned char *tff_buffer = load_file_into_mem_u(filepath);

  stbtt_fontinfo info;
  if (!stbtt_InitFont(&info, tff_buffer, 0)) {
    fprintf(stderr, "unable to load ttf file from %s", filepath);
  }

  free(tff_buffer);
}
