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
  Vec3 m_ambient;          // Ka
  Vec3 m_diffuse;          // Kd
  Vec3 m_specular;         // Ks
  float m_specular_exp;    // specular exponent
  Texture m_map_diffuse; 
  Texture m_map_bump;
  Texture m_map_specular;
};

#endif  // MATERIAL_H_
