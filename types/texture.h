#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <stb_image.h>

/**
 * @brief An image texture.
 */
struct Texture {
  int m_width;
  int m_height;
  int m_num_channels;
  unsigned char *mp_data = nullptr;

  /**
   * @brief Free image memory.
   */
  ~Texture();
};

#endif  // TEXTURE_H_
