#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <vector>

#include "texture.h"
#include <vec3.h>

/**
 * @brief A material is an array of textures.
 *        We assume these images will never be used outside of a material.
 */
struct Material {
  float m_specular_exp;   // specular exponent
  Texture m_map_diffuse;  // map_Kd
  Texture m_map_bump;     // map_Bump
  Texture m_map_specular; // map_Ks
};

#endif  // MATERIAL_H_
