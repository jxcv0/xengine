#include "mesh.h"
#include <cstdint>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void
Mesh::gen_buffers() {
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  glGenBuffers(1, &m_EBO);

  glBindVertexArray(m_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
      m_vertices.data(), GL_STATIC_DRAW);

  // indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               m_indices.size() * sizeof(uint32_t),
               m_indices.data(), GL_STATIC_DRAW); 

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

  // texture coords
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (void*)offsetof(Vertex, tex_coord));

  // vertex normals
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (void*)offsetof(Vertex, normal));

  // tangent
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (void*)offsetof(Vertex, tangent));

  // bitangent
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (void*)offsetof(Vertex, bitangent));

  // unbind
  glBindVertexArray(0);
}

namespace MeshUtils {
  /*! \brief      Load an image texture from a path and add it to GL texture buffer.
   *  \param path The filepath to the image file.
   *  \return     A handle to the texture object.
   */
	uint32_t load_texture(const char* path) {
		uint32_t texture_id;
		glGenTextures(1, &texture_id);

		int width, height, num_comp;
		unsigned char* data = stbi_load(path, &width, &height, &num_comp, 0);

		if (data) {
			GLenum format;
			if (num_comp == 1) {
				format = GL_RED;
			} else if (num_comp == 3) {
				format = GL_RGB;
			} else if (num_comp == 4) {
				format = GL_RGBA;
			}

			glBindTexture(GL_TEXTURE_2D, texture_id);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		} else {
			std::cout << "Unable to load texture from " << path << "\n";
		}

    stbi_image_free(data);
		return texture_id;
  }
} // namespace MeshUtils
