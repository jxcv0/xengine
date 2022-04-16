#ifndef MODEL_H
#define MODEL_H

#include <glad.h>

#include <vector>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <fstream>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include "shader.h"

namespace xen
{
	// load a texture from a file and bind to gl texture buffer
	unsigned int loadTextureFromFile(const char* path)
	{
		unsigned int texId;
		glGenTextures(1, &texId);

		int width, height, numComp;
		unsigned char* data = stbi_load(path, &width, &height, &numComp, 0);

		if (data)
		{
			GLenum format;
			if (numComp == 1)
			{
				format = GL_RED;
			}
			else if (numComp == 3)
			{
				format = GL_RGB;
			}
			else if (numComp == 4)
			{
				format = GL_RGBA;
			}

			glBindTexture(GL_TEXTURE_2D, texId);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			// glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "ERROR::TEXTURE_LOAD_ERR: Unable to load texture from " << path << "\n";
			stbi_image_free(data);
		}

		return texId;
	}

	// an individual vertex
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	// a texture
	struct Texture
	{
		unsigned int id;
		std::string type;
	};

	// mesh data
	struct Mesh
	{
		unsigned int VAO, VBO, EBO;		// gl buffer handles
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
	};

	// a model is a collection of meshes and the data required for generating a model matrix
	struct Model
	{
		glm::vec3 position = glm::vec3(0.0f);
		std::vector<Mesh> meshes;
		// TODO rotation and scale
	};

	void loadObj(const char* path)
	{
		std::ifstream file(path);
		std::string line;
		while (std::getline(file, line))
		{
			if (line.starts_with('#')) { break; };	// assuming comments are always alone on a line

			if (line.starts_with("v"))
			{
				size_t pos = 0;
				while ((pos = line.find(" ")) != std::string::npos)
				{
					// continue from here
				}
			}
		}
	}

	void genModelBuffers(Model &model)
	{
		for (auto &mesh : model.meshes)
		{
			glGenVertexArrays(1, &mesh.VAO);
			glGenBuffers(1, &mesh.VBO);
			glGenBuffers(1, &mesh.EBO);

			glBindVertexArray(mesh.VAO);

			glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
			glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW); 

			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

			// texture coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

			// unbind
			glBindVertexArray(0);
		}
	}

	// draw a mesh using a shader program
	void drawMesh(Mesh &mesh)
	{
		// TODO textures
		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh.indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	// draw all meshes in a model using a single shader program
	void drawModel(Model& model)
	{

		for (size_t i = 0; i < model.meshes.size(); i++)
		{
			drawMesh(model.meshes[i]);
		}
	}

	// generate model matrix based on model position
	glm::mat4 modelMatrix(Model &model)
	{
		return glm::translate(glm::mat4(1.0f), model.position);
	}
}

#endif // MODEL_H
