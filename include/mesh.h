#ifndef MESH_H_
#define MESH_H_

#include <string>
#include <vector>
#include <cstdint>

#include <glm/glm.hpp>

#ifndef OPENGL_LIBS
#define OPENGL_LIBS
#include <glad.h>
#endif

/*! \brief A Mesh and its textures.
 */
class Mesh {

  /*! \brief A single vertex.
   */
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 tex_coord;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};

  /*! \brief An image texture
   */
	struct Texture {
		unsigned int id;
		std::string uniform_name;
	};

  std::string m_filepath;
  uint32_t m_VAO;
  uint32_t m_VBO;
  uint32_t m_EBO;
  std::vector<Vertex> m_vertices;
  std::vector<uint32_t> m_indices;
  std::vector<Texture> m_textures;

public:

  Mesh() = delete;

  /*! \brief          Construct a mesh with a filepath.
   *                  The mesh is not loaded from file here.
   *  \param filepath The filepath to the 3D mesh file.
   */
  Mesh(const char *filepath) : m_filepath(filepath) {};

  ~Mesh() {
  }

  /*! \brief Load the mesh data from the file.
   */
  void load_from_file();

  /*! \brief Generate OpenGL buffer objects for this mesh.
   */
	void gen_buffers();
};

/* Why is this here??
struct Light {
  glm::vec3 colour = glm::vec3(1.0f);
  glm::vec3 position;

  // attenuation
  float constant = 1.0f;
  float linear = 0.09f;
  float quadratic = 0.032f;
};
*/

namespace MeshUtils {

  /*! \brief      Load an image texture from a path and add it to GL texture buffer.
   *  \param path The filepath to the image file.
   *  \return     A handle to the texture object.
   */
	uint32_t load_texture(const char* path);

/* TODO This should be part of a system
	// draw all meshes in a model using a single shader program
	void draw_model(unsigned int model, unsigned int shader)
	{
        for (int i = 0; i < _models[model].numTextures; i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            xen::shader::set_uniform(shader, _models[model].textures[i].uniformName.c_str(), i);
            glBindTexture(GL_TEXTURE_2D, _models[model].textures[i].id);
        }
        glBindVertexArray(_models[model].VAO);
        glDrawElements(GL_TRIANGLES, _models[model].numIndices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
	}
*/
} // namespace MeshUtils

#endif // MESH_H_
