#ifndef MODEL_H
#define MODEL_H

#include <glad.h>

#include <vector>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
		std::string name;			// name of model used to load textures
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

	// temporary storage for tracking what files have already been loaded into GL_TEXTURE_*
	struct TextureCache
	{
		unsigned int id;
		std::string path;
	};

	// load a texture file into a mesh only if it has not already been buffered into GL
	std::vector<Texture> loadMaterialTexture(aiMaterial *material, aiTextureType type, const char* uniformName, std::vector<TextureCache> &cache)
	{
		std::vector<Texture> textures;
		for (size_t i = 0; i < material->GetTextureCount(type); i++)
		{
			aiString str;
			material->GetTexture(type, i, &str);
			bool skip = false;

			for (auto &c : cache)
			{
				if (std::strcmp(c.path.data(), str.C_Str()) == 0)
				{
					Texture texture = { c.id };
					textures.push_back(texture);
					skip = true;
					break;
				}
			}

			if (!skip)
			{
				Texture texture;
				texture.id = loadTextureFromFile(str.C_Str());
				texture.uniformName = uniformName;
				
				textures.push_back(texture);
				
				TextureCache c = { texture.id, str.C_Str() };
				cache.push_back(c);
			}
		}

		return textures;
	}

	// process imported mesh data
	Mesh processMesh(aiMesh *mesh, const aiScene *scene)
	{
		Mesh xMesh;

		// vertices
		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			// position
			Vertex vertex;
			glm::vec3 vector3;
			vector3.x = mesh->mVertices[i].x;
			vector3.y = mesh->mVertices[i].y;
			vector3.z = mesh->mVertices[i].z;
			vertex.position = vector3;

			// normals
			if (mesh->HasNormals())
			{
				vector3.x = mesh->mNormals[i].x;
				vector3.y = mesh->mNormals[i].y;
				vector3.z = mesh->mNormals[i].z;
				vertex.normal = vector3;
			}

			// texCoord
			if (mesh->mTextureCoords[0]) // each vertex may only have one set of texture coordinates for now
			{
				glm::vec2 vector2;
				vector2.x = mesh->mTextureCoords[0][i].x;
				vector2.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoord = vector2;
			}
			else
			{
				vertex.texCoord = glm::vec2(0.0f, 0.0f);

			}

			xMesh.vertices.push_back(vertex);
		}

		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			for (size_t j = 0; j < mesh->mFaces[i].mNumIndices; j++)
			{
				xMesh.indices.push_back(mesh->mFaces[i].mIndices[j]);
			}
		}
		
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<TextureCache> cache;

		std::vector<Texture> diffuseMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse", cache);
		xMesh.textures.insert(xMesh.textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		
		std::vector<Texture> specularMaps = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular", cache);
		xMesh.textures.insert(xMesh.textures.end(), specularMaps.begin(), diffuseMaps.end());

		std::vector<Texture> normalMaps = loadMaterialTexture(material, aiTextureType_HEIGHT, "texture_normal", cache);
		xMesh.textures.insert(xMesh.textures.end(), normalMaps.begin(), normalMaps.end());

		return xMesh;
	}

	// recursively process all node in a scene
	void processNode(Model &model, aiNode *node, const aiScene *scene)
	{
		for(size_t i = 0; i < node->mNumMeshes; i++)
		{
			model.meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));
		}
			
		for(size_t i = 0; i < node->mNumChildren; i++)
		{
			processNode(model, node->mChildren[i], scene);
		}
	}

	// load data from a file into a model
	void loadModel(Model &model, const char* path)
	{
		Assimp::Importer imp;
		const aiScene *scene = imp.ReadFile(path,
				aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP:: " << imp.GetErrorString() << "\n";
			return;
		}
		processNode(model, scene->mRootNode, scene);
	}

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
		for (size_t i = 0; i < model.meshes.size(); i++) { drawMesh(model.meshes[i]); }
	}

	// generate model matrix based on model position
	glm::mat4 modelMatrix(Model &model)
	{
		return glm::translate(glm::mat4(1.0f), model.position);
	}
}

#endif // MODEL_H
