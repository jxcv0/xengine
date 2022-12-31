#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "stb_image.h"

/**
 * @brief An image texture.
 */
struct texture {
  int m_width;
  int m_height;
  int m_num_channels;
  unsigned char *mp_data;
};

#endif  // TEXTURE_H_
