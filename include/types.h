#ifndef TYPES_H_
#define TYPES_H_

#include <vector>
#include <string>
#include <cstdint>

#include <glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using eid_t = uint32_t;
using cid_t = uint32_t;

/** @brief A single vertex.
 */
struct Vertex { glm::vec3 m_position;
  glm::vec3 m_normal;
  glm::vec2 m_tex_coord;
  glm::vec3 m_tangent;
  glm::vec3 m_bitangent;
};

/** @brief An image texture
 */
struct Texture {
  std::byte *mp_data;

  ~Texture() {
    stbi_image_free(mp_data);
  }
};

/** @brief A material component consisting of 3 texure types.
 */
struct Material {
  Texture *mp_diffuse;
  Texture *mp_specular;
  Texture *mp_ambient;
};


/** @brief A Mesh component. This may in fact be a collection of different
 *         Meshes from the same directory.
 */
struct Mesh {
  std::vector<Vertex> m_vertices;
  std::vector<uint32_t> m_indices;
};

/** @brief A Model is a collection of meshes.
 */
struct Model {
  std::vector<Mesh> m_meshes;
};

/** @brief A basic light source.
 *         TODO all of it.
 */
struct Light {
  glm::vec3 colour = glm::vec3(1.0f);
  glm::vec3 position;

  // attenuation
  float constant = 1.0f;
  float linear = 0.09f;
  float quadratic = 0.032f;
};

#endif // TYPES_H_
