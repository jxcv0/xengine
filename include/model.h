#ifndef MESH_H_
#define MESH_H_

#include <string>
#include <vector>
#include <cstdint>

#include <glm/glm.hpp>
#include <assimp/scene.h>

#ifndef OPENGL_LIBS
#define OPENGL_LIBS
#include <glad.h>
#endif

/*! \brief A single vertex.
 */
struct Vertex {
  glm::vec3 m_position;
  glm::vec3 m_normal;
  glm::vec2 m_tex_coord;
  glm::vec3 m_tangent;
  glm::vec3 m_bitangent;
};

/*! \brief An image texture
 */
struct Texture {
  unsigned int id;
  std::string m_uniform_name;
};

/*! \brief A Mesh and its textures.
 */
class Mesh {
  std::string filepath;
  uint32_t m_VAO;
  uint32_t m_VBO;
  uint32_t m_EBO;
  std::vector<Vertex> m_vertices;
  std::vector<uint32_t> m_indices;
  std::vector<Texture> m_textures;

public:

  /*! \brief Default constructor so that Mesh can be used in containers without
   *         The need for loading from file.
   */
  Mesh() = default;

  /*! \brief Default destructor.
   */
  ~Mesh() = default;

  /*! \brief Construct a mesh from preloaded mesh data.
   *  \param vertices A vector of vertices.
   *  \param indices A vector of indices.
   *  \param textures A vector of textures.
   */
  Mesh(std::vector<Vertex> vertices,
       std::vector<uint32_t> indices,
       std::vector<Texture> textures)
    : m_vertices(vertices)
    , m_indices(indices)
    , m_textures(textures) {};


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

/*! \brief Most models are a collection of multiple meshes.
 *         Not to be confused with MVC model.
 */
class Model {
  std::vector<Mesh> m_meshes;

  /*! \brief Recersively process nodes in a model file and the files in the same
   *         directory. This is used when loading a model with Assimp.
   *  \param dir The directory of the object file.
   *  \param node The aiNode.
   *  \param scene The aiScene.
   */
  void process_node(const char *dir, aiNode *node, const aiScene *scene);

  /*! \brief Convert an aiMesh into a Mesh.
   *  \param dir The directory of the mesh files.
   *  \param mesh The aiMesh.
   *  \param scene The aiScene.
   */
  void process_mesh(const char *dir, aiMesh *mesh, const aiScene *scene);

public:
  /*! \brief Default constructor so that loading from file can be deferred.
   */
  Model() = default;

  /*! \brief Default destructor.
   */
  ~Model() = default;

  /*! \brief Load the mesh data from the file.
   *  \param filepath The path to th 3d model file.
   *
   *  TODO This should be the responsibility of some other system.
   *       Model should only be concerned with managing meshes in memory.
   */
  void load(const char *filepath);
};

namespace MeshUtils {

  /*! \brief      Load an image texture from a path and add it to GL texture buffer.
   *  \param path The filepath to the image file.
   *  \return     A handle to the texture object.
   */
	uint32_t load_texture(const char* path);

/* TODO This should be part of a RenderingSystem
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
