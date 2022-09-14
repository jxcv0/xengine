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

  uint32_t m_VAO;
  uint32_t m_VBO;
  uint32_t m_EBO;
  std::vector<Vertex> m_vertices;
  std::vector<uint32_t> m_indices;
  std::vector<Texture> m_textures;

public:

  /*! \brief TODO
   */
  Mesh();

  /*! \brief TODO
   */
  ~Mesh();

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

/*! \brief Load a mesh from a file
 */
Mesh* load_mesh(const char* filepath) {
	Mesh* mesh = new Mesh();

	// open file
	FILE *file = NULL;
	if ((file = fopen(filepath, "r")) == NULL) {
		perror(filepath);
		return -1;
	}

	int v_count = 0, vt_count = 0, vn_count = 0, f_count = 0;
	char *line = NULL;
	size_t len = 0;
	ssize_t nread = 0;
	while ((nread = getline(&line, &len, file)) != -1) // count the number of vectors
	{
		if (strstr(line, "v ") != NULL) {
			v_count++;
		} else if (strstr(line, "vt ") != NULL) {
			vt_count++;
		} else if (strstr(line, "vn ") != NULL) {
			vn_count++;
		} else if (strstr(line, "f ") != NULL) {
			f_count++;
		}
	}

	// temp storage
	vec3_t vertices[v_count];
	vec2_t texcoords[vt_count];
	vec3_t normals[vn_count];
	// allocate enough space for duplicating VTN sets
	mesh->num_vertices = f_count * 3; // 3 vertices per face
	size_t vertices_size = sizeof(vec3_t) * mesh->num_vertices;
	size_t texcoords_size = sizeof(vec2_t) * mesh->num_vertices;
	size_t normals_size = sizeof(vec3_t) * mesh->num_vertices;
	size_t mem_block_size = vertices_size + texcoords_size + normals_size;
	mesh->mem_block = malloc(mem_block_size);
	if (mesh->mem_block == NULL) {
		mesh = NULL;
		return -1;
	}
	mesh->vertices = (vec3_t*)(mesh->mem_block);
	mesh->texcoords = (vec2_t*)(mesh->mem_block + vertices_size);
	mesh->normals = (vec3_t*)(mesh->mem_block + vertices_size + normals_size);
	v_count = 0;
	vt_count = 0;
	vn_count = 0;
	int it = 0;
	rewind(file);

	while ((nread = getline(&line, &len, file)) != -1)
	{
		char *linesave = NULL;
		char *token = strtok_r(line, " ", &linesave);
		if (strncmp(token, "#", 2) == 0) { // comments
			continue;
		} else if (strncmp(token, "mtllib", 6) == 0) { // material library
			// TODO handle materials
			continue;
		} else if (strncmp(token, "o", 2) == 0) { // object name
			continue;
		} else if (strncmp(token, "v", 2) == 0) { // vertices
			for(int i = 0; ; i++)
			{
				token = strtok_r(NULL, " ", &linesave);
				if (token == NULL) { break; }
				vertices[v_count].values[i] = strtof(token, NULL);
			}
			v_count++;
		} else if (strncmp(token, "vt", 2) == 0) { // texcoords
			for(int i = 0; ; i++)
			{
				token = strtok_r(NULL, " ", &linesave);
				if (token == NULL) { break; }
				texcoords[vt_count].values[i] = strtof(token, NULL);
			}
			vt_count++;
		} else if (strncmp(token, "vn", 2) == 0) { // normals
			for(int i = 0; ; i++)
			{
				token = strtok_r(NULL, " ", &linesave);
				if (token == NULL) { break; }
				normals[vn_count].values[i] = strtof(token, NULL);
			}
			vn_count++;
		} else if (strncmp(token, "s", 2) == 0) { // smooth shading always off
			continue;
		} else if (strncmp(token, "f", 2) == 0) { // faces
			char *toksave = NULL;
			for(; it < f_count; it++)
			{
				token = strtok_r(NULL, " ", &linesave);
				if (token == NULL) { break; }
				int  i = 0;
				int index[3];
				for (char* tok = token; ; i++, tok = NULL)
				{
					char* subtok = strtok_r(tok, "/", &toksave);
					if (subtok == NULL) { break; }
					index[i] = atoi(subtok) - 1;
				}
				mesh->vertices[it] = vertices[index[0]];
				mesh->texcoords[it] = texcoords[index[1]];
				mesh->normals[it] = normals[index[2]];
			}
		} else if (strncmp(token, "mtllib", 6) == 0) {
			// TODO handle materials
			continue;
		}
	}
	free(line);
	fclose(file);

	glGenBuffers(1, &mesh->VBO);
	glGenVertexArrays(1, &mesh->VBO);
	glEnableVertexAttribArray(mesh->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER, mem_block_size, mesh->vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, vertices_size, GL_FLOAT, GL_FALSE, 0, mesh->vertices);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, normals_size, GL_FLOAT, GL_FALSE, 0, mesh->normals);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, texcoords_size, GL_FLOAT, GL_FALSE, 0, mesh->texcoords);

	return num_meshes++;
} // namespace MeshUtils

#endif // MESH_H_
