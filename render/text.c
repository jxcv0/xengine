#include "text.h"

#include <stdio.h>
#include <stdlib.h>

#include "glad.h"
#include "stb_truetype.h"
#include "utils.h"

struct character debug_chars[128];

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
}

void text_debug(vec2 position, const char *txt) {
  (void)position;
  (void)txt;
}

void clean_up(void) {
  for (int i = 0; i < 128; i++) {
    stbtt_FreeBitmap(debug_chars[i].mp_bitmap, NULL);
  }
}
