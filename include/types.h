#ifndef TYPES_H_
#define TYPES_H_

#include <vector>
#include <string>
#include <cstdint>

#include <glm/glm.hpp>

using eid_t = uint32_t;
using cid_t = uint32_t;

/**
 * @brief A single vertex.
 */
struct Vertex { glm::vec3 m_position;
  glm::vec3 m_normal;
  glm::vec2 m_tex_coord;
  glm::vec3 m_tangent;
  glm::vec3 m_bitangent;
};

enum class TextureType {
  diffuse,
  specular,
  normal,
  height
};

/**
 * @brief An image texture
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

/**
 * @brief A material is an array of textures.
 *        We assume these images will never be used outside of a material.
 */
struct Material {
  std::vector<Texture> m_textures;
};


/**
 * @brief A Mesh component. This may in fact be a collection of different
 *        Meshes from the same directory.
 */
struct Mesh {
  std::vector<Vertex> m_vertices;
  std::vector<uint32_t> m_indices;
};

/**
 * @brief A Model is a collection of meshes.
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
