#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "stb_image.h"

/**
 * @brief The texture type.
 */
enum class TextureType { diffuse, specular, normal, height };

/**
 * @brief An image texture.
 */
struct Texture {
  int m_width;
  int m_height;
  int m_num_channels;
  unsigned char *mp_data = nullptr;
  TextureType m_type;

  /**
   * @brief Free image memory.
   */
  ~Texture() {
    if (mp_data != nullptr) {
      stbi_image_free(mp_data);
    }
  }
};

#endif  // TEXTURE_H_
