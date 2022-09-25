#ifndef TEXTURE_H_
#define TEXTURE_H_

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
  unsigned char *mp_data;
  TextureType m_type;

  /**
   * @brief Free image memory.
   */
  ~Texture();
};

#endif  // TEXTURE_H_
