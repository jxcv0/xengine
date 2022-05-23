#ifndef SCENE_H
#define SCENE_H

#include <glad.h>

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// #include <glm/glm.hpp>

#include "shader.h"
#include "alloc.h"

#define XEN_MAX_MODELS 256
#define XEN_MAX_LIGHTS 32

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

	// TODO animations
	struct Model
	{
		glm::vec3 position{0.0f, 0.0f, 0.0f};
		const glm::vec3 y{0.0f, 0.0f, 0.0f};
		float b = 0.0f;						                // rotation about global y axis (up)
        glm::mat4 matrix;

        // mesh data
		unsigned int VAO, VBO, EBO;

        size_t numVertices = 0;
        Vertex* vertices;

        size_t numIndices = 0;
		unsigned int* indices;

        size_t numTextures = 0;
		Texture textures[3]; // diffuse, specular, normal
	};

	struct Light
	{
		glm::vec3 colour = glm::vec3(1.0f);
		glm::vec3 position;

		// attenuation
		float constant = 1.0f;
		float linear = 0.09f;
		float quadratic = 0.032f;
	};
	
    size_t _modelMkr = 0;
    Model _models[XEN_MAX_MODELS];

    size_t _lightMkr = 0;
    Light _lights[XEN_MAX_LIGHTS];

    // stack allocators
    xen::mem::StackAllocator<Vertex> _vertexAllocator(20000);
    xen::mem::StackAllocator<unsigned int> _indexAllocator(20000);
} // namespace

namespace xen::scene
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
            model->textures[model->numTextures++] = texture;
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
            model->numIndices = mesh->mNumFaces * 3; // all faces are triangulated
            model->indices = _indexAllocator.allocate(model->numIndices);

			for(size_t j = 0; j < mesh->mNumVertices; j++)
			{
				Vertex vertex;
				glm::vec3 vector;

				// positions
				vector[0] = mesh->mVertices[j].x;
				vector[1] = mesh->mVertices[j].y;
				vector[2] = mesh->mVertices[j].z;
				vertex.position = vector;

				// normals
				if (mesh->HasNormals())
				{
					vector[0] = mesh->mNormals[j].x;
					vector[1] = mesh->mNormals[j].y;
					vector[2] = mesh->mNormals[j].z;
					vertex.normal = vector;
				}

				// texture coordinates
				if(mesh->mTextureCoords[0])
				{
					glm::vec2 vec;
					vec[0] = mesh->mTextureCoords[0][j].x; 
					vec[1] = mesh->mTextureCoords[0][j].y;
					vertex.texCoord = vec;

					vector[0] = mesh->mTangents[i].x;
					vector[1] = mesh->mTangents[i].y;
					vector[2] = mesh->mTangents[i].z;
					vertex.tangent = vector;

					vector[0] = mesh->mBitangents[i].x;
					vector[1] = mesh->mBitangents[i].y;
					vector[2] = mesh->mBitangents[i].z;
					vertex.bitangent = vector;
				}
				else
				{
					vertex.texCoord = {0.0f, 0.0f};
				}
                model->vertices[j] = vertex;
			}

            size_t temp = 0;
			// indices
			for(size_t j = 0; j < mesh->mNumFaces; j++)
			{
				aiFace face = mesh->mFaces[j];

				for(size_t k = 0; k < face.mNumIndices; k++)
				{
                    model->indices[temp++] = face.mIndices[k];
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

	// load a model from a file and return a handle to the model in the scene
	int load_model(const char* filepath)
	{
        if (_modelMkr == XEN_MAX_MODELS) { return -1; }

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

		process_node(&_models[_modelMkr], scene->mRootNode, scene, dir);
        return _modelMkr++;
	}
	
	// buffer model data in gl
    // TODO move to render.h
	void gen_buffers(unsigned int model)
	{
        glGenVertexArrays(1, &_models[model].VAO);
        glGenBuffers(1, &_models[model].VBO);
        glGenBuffers(1, &_models[model].EBO);

        glBindVertexArray(_models[model].VAO);

        glBindBuffer(GL_ARRAY_BUFFER, _models[model].VBO);
        glBufferData(GL_ARRAY_BUFFER, _models[model].numVertices * sizeof(Vertex), &_models[model].vertices[0], GL_STATIC_DRAW);

        // indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _models[model].EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _models[model].numIndices * sizeof(unsigned int), &_models[model].indices[0], GL_STATIC_DRAW); 

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
    
    // get a ptr to a model position
    // used for if a game entity needs a reference to another model for calculations
    Model* get_model(unsigned int model)
    {
        return &_models[model];
    }

	// generate model matrix based on model position
	void update_model_matrix(unsigned int model)
	{
		auto mat = glm::translate(glm::mat4(1.0f), _models[model].position);
		_models[model].matrix = glm::rotate(mat, glm::radians(_models[model].b), glm::vec3(0.0f, 1.0f, 0.0f));
	}

    // update and get a model matrix
    glm::mat4 model_matrix(unsigned int model)
    {
        update_model_matrix(model); // keep this?
        return _models[model].matrix;
    }

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

    // set the rotation of the model about the global y axis
    inline void set_model_rotation_y(unsigned int model, float r)
    {
        _models[model].b = r;
    }

    inline int add_light(glm::vec3 pos)
    {
        _lights[_lightMkr].position = pos;
        return _lightMkr++;
    }

    // temp function just to move this here
    // TODO delete this and set up better system of setting light uniforms
    void set_light_uniforms(unsigned int shader, unsigned int light)
    {
		xen::shader::set_uniform(shader, "light.position", _lights[light].position);
		xen::shader::set_uniform(shader, "light.colour", _lights[light].colour);
		xen::shader::set_uniform(shader, "light.constant", _lights[light].constant);
		xen::shader::set_uniform(shader, "light.linear", _lights[light].linear);
		xen::shader::set_uniform(shader, "light.quadratic", _lights[light].quadratic);
    }
} //namespace xen::scene

#endif // SCENE_H
