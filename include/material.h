#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <vector>
#include "texture.h"

/**
 * @brief A material is an array of textures.
 *        We assume these images will never be used outside of a material.
 */
struct Material {
  std::vector<Texture> m_textures;
};

#endif // MATERIAL_H_
