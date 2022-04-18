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
	// an individual vertex
	struct Vertex
	{
		// TODO tangent, bitangent
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	// a texture
	struct Texture
	{
		unsigned int id;
		std::string uniformName;
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
		glm::vec3 position = glm::vec3(0.0f);	// position of model in world space
		std::vector<Mesh> meshes;		// the meshes comprising the model
		// TODO rotation and scale
	};

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

			// TODO multisampling
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

	// load all textures of a type into a mesh
	void loadMaterial(Mesh &mesh, std::string &dir, aiMaterial *mat, aiTextureType type, std::string typeName)
	{
		for (size_t i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			Texture texture;
			std::string fileName(str.C_Str());
			texture.id = loadTextureFromFile((dir + "/" + fileName).c_str());
			texture.uniformName = typeName;
			mesh.textures.push_back(texture);
		}
	};

	// recursively process nodes in a scene
	void processNode(Model &model, aiNode *node, const aiScene *scene, std::string dir)
	{
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			Mesh xenMesh;
			for(size_t j = 0; j < mesh->mNumVertices; j++)
			{
				Vertex vertex;
				glm::vec3 vector;

				// positions
				vector.x = mesh->mVertices[j].x;
				vector.y = mesh->mVertices[j].y;
				vector.z = mesh->mVertices[j].z;
				vertex.position = vector;

				// normals
				if (mesh->HasNormals())
				{
					vector.x = mesh->mNormals[j].x;
					vector.y = mesh->mNormals[j].y;
					vector.z = mesh->mNormals[j].z;
					vertex.normal = vector;
				}

				// texture coordinates
				if(mesh->mTextureCoords[0])
				{
					glm::vec2 vec;
					vec.x = mesh->mTextureCoords[0][j].x; 
					vec.y = mesh->mTextureCoords[0][j].y;
					vertex.texCoord = vec;
				}
				else
				{
					vertex.texCoord = glm::vec2(0.0f, 0.0f);
				}
				xenMesh.vertices.push_back(vertex);
			}

			// indices
			for(size_t j = 0; j < mesh->mNumFaces; j++)
			{
				aiFace face = mesh->mFaces[j];
				for(size_t k = 0; k < face.mNumIndices; k++)
				{
					xenMesh.indices.push_back(face.mIndices[k]);        
				}
			}

			// textures
			aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

			loadMaterial(xenMesh, dir, material, aiTextureType_DIFFUSE, "textureDiffuse");
			loadMaterial(xenMesh, dir, material, aiTextureType_SPECULAR, "textureSpecular");
			loadMaterial(xenMesh, dir, material, aiTextureType_HEIGHT, "textureNormal");

			model.meshes.push_back(xenMesh);
		}

		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			processNode(model, node->mChildren[i], scene, dir);
		}
	}

	// load a model from a file
	void loadModel(Model &model, const char* filepath)
	{
		std::string dir(filepath);
		dir = dir.substr(0, dir.find_last_of('/'));

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filepath,
				aiProcess_Triangulate |
				aiProcess_GenSmoothNormals |
				aiProcess_FlipUVs |
				aiProcess_CalcTangentSpace);

		if(!scene)
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << "\n";
			return;
		}

		processNode(model, scene->mRootNode, scene, dir);
	}

	// buffer model data in gl
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

			// indices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW); 

			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

			// texture coords
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

			// vertex normals
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

			// unbind
			glBindVertexArray(0);
		}
	}

	// draw a mesh using a shader program
	// this function assumes each mesh only has one diffuse, specular and normal map
	void drawMesh(Mesh &mesh, unsigned int shader)
	{
		for (int i = 0; i < mesh.textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			setShaderUniform(shader, mesh.textures[i].uniformName.c_str(), i);
			glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
		}
		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh.indices.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

	// draw all meshes in a model using a single shader program
	void drawModel(Model& model, unsigned int shader)
	{
		for (auto &mesh : model.meshes) { drawMesh(mesh, shader); }
	}

	// generate model matrix based on model position
	glm::mat4 modelMatrix(Model &model)
	{
		return glm::translate(glm::mat4(1.0f), model.position);
	}
}

#endif // MODEL_H
