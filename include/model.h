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
#include "alloc.h"

#define XEN_MAX_MODELS 256

namespace
{
	// an individual vertex
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};
	
	// a texture
	struct Texture
	{
		unsigned int id;
		std::string uniformName;
	};

	// a model is a collection of meshes and the data required for generating a model matrix
	// TODO animations
	struct Model
	{
		glm::vec3 position = glm::vec3(0.0f);			    // position of model in world space - default to world 0, 0, 0
		const glm::vec3 y = glm::vec3(0.0f, 1.0f, 0.0f);
		float b = 0.0f;						                // rotation about global y axis (up)
        glm::mat4 matrix;

        // mesh
		unsigned int VAO, VBO, EBO;

        size_t numVertices;
        Vertex* vertices;

		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
        // Mesh* meshes;
	};
	
    // up to 256 models per scene
    Model _models[XEN_MAX_MODELS];
    size_t _mkr;

    // stack allocators
    xen::mem::StackAllocator<Vertex> _vertexAllocator(20000); // this is only enough for one model!!
    xen::mem::StackAllocator<Texture> _textureAllocator(XEN_MAX_MODELS * 3); // diffuse, normal, specular
    xen::mem::StackAllocator<unsigned int> _indexAllocator(XEN_MAX_MODELS);

} // namespace

namespace xen::model
{
	// load a texture from a file and bind to gl texture buffer
	unsigned int load_texture(const char* path)
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

			// TODO multisampling for antiailiasing
            // and futher reading on adding and removing data from GL_TEXTURE_2D
			glBindTexture(GL_TEXTURE_2D, texId);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

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
	void load_material(Model* model, std::string &dir, aiMaterial *mat, aiTextureType type, std::string typeName)
	{
		for (size_t i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			Texture texture;
			std::string fileName(str.C_Str());
			texture.id = load_texture((dir + "/" + fileName).c_str());
			texture.uniformName = typeName;
			model->textures.push_back(texture);
		}
	};

	// recursively process nodes in a scene
	void process_node(Model* model, aiNode *node, const aiScene *scene, std::string dir)
	{
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

            model->numVertices = mesh->mNumVertices;
            model->vertices = _vertexAllocator.allocate(model->numVertices);

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

					vector.x = mesh->mTangents[i].x;
					vector.y = mesh->mTangents[i].y;
					vector.z = mesh->mTangents[i].z;
					vertex.tangent = vector;

					vector.x = mesh->mBitangents[i].x;
					vector.y = mesh->mBitangents[i].y;
					vector.z = mesh->mBitangents[i].z;
					vertex.bitangent = vector;
				}
				else
				{
					vertex.texCoord = glm::vec2(0.0f, 0.0f);
				}
                model->vertices[j] = vertex;
			}

			// indices
			for(size_t j = 0; j < mesh->mNumFaces; j++)
			{
				aiFace face = mesh->mFaces[j];
				for(size_t k = 0; k < face.mNumIndices; k++)
				{
				    model->indices.push_back(face.mIndices[k]);        
				}
			}
            
			// textures
			aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

			load_material(model, dir, material, aiTextureType_DIFFUSE, "textureDiffuse");
			load_material(model, dir, material, aiTextureType_SPECULAR, "textureSpecular");
			load_material(model, dir, material, aiTextureType_HEIGHT, "textureNormal");
		}

		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			process_node(model, node->mChildren[i], scene, dir);
		}
	}

	// load a model from a file
    // return a handle to the model in the scene
	int load(const char* filepath)
	{
        if (_mkr == XEN_MAX_MODELS) { return -1; }

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
			return -1;
		}

		process_node(&_models[_mkr], scene->mRootNode, scene, dir);
        return _mkr++;
	}
	
	// buffer model data in gl
    // TODO move to render.h
	void gen_buffers(unsigned int handle)
	{
        glGenVertexArrays(1, &_models[handle].VAO);
        glGenBuffers(1, &_models[handle].VBO);
        glGenBuffers(1, &_models[handle].EBO);

        glBindVertexArray(_models[handle].VAO);

        glBindBuffer(GL_ARRAY_BUFFER, _models[handle].VBO);
        glBufferData(GL_ARRAY_BUFFER, _models[handle].numVertices * sizeof(Vertex), &_models[handle].vertices[0], GL_STATIC_DRAW);

        // indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _models[handle].EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _models[handle].indices.size() * sizeof(unsigned int), &_models[handle].indices[0], GL_STATIC_DRAW); 

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // texture coords
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

        // vertex normals
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        // tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

        // bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

        // unbind
        glBindVertexArray(0);
	}

	// generate model matrix based on model position
	void update_model_matrix(unsigned int handle)
	{
		auto mat = glm::translate(glm::mat4(1.0f), _models[handle].position);
		_models[handle].matrix = glm::rotate(mat, glm::radians(_models[handle].b), glm::vec3(0.0f, 1.0f, 0.0f));
	}

    glm::mat4 model_matrix(unsigned int model)
    {
        update_model_matrix(model);
        return _models[model].matrix;
    }

	// draw all meshes in a model using a single shader program
	void draw(unsigned int handle, unsigned int shader)
	{
        for (int i = 0; i < _models[handle].textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            xen::shader::set_uniform(shader, _models[handle].textures[i].uniformName.c_str(), i);
            glBindTexture(GL_TEXTURE_2D, _models[handle].textures[i].id);
        }
        glBindVertexArray(_models[handle].VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(_models[handle].indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
	}
} //namespace xen::model

#endif // MODEL_H
