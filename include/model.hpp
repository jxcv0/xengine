#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>
#include <iostream>

// TODO (distant future) dynamically allocate these at save/level loading stage
// TODO replace std::vector with more efficient type (array?)

namespace xen {
    
    /**
     * @brief Contains mesh data imported from asset files
     * 
     */
    struct Model {


        /**
         * @brief Mesh vertex data
         * 
         */
        struct Vertex {
            glm::vec3 position;     // vertex position in model space
            glm::vec3 normal;       // normal vector of this vertex (will this lead to duplicate data?)
            glm::vec2 tex_coords;   // texture coords 
        };

        /**
         * @brief Model texture data
         * 
         */
        struct Texture {
            unsigned int id;    // texture id for binding to buffer
            std::string type;   // texture type
            std::string path;   // path to texture
        };

        struct Mesh {
            unsigned int VAO, VBO, EBO;         // openGL buffer handles
            std::vector<Vertex> vertices;       // self explanatory
            std::vector<unsigned int> indices;  // index for EBO
            // std::vector<Texture> textures;
        };

        // TODO - textures
        // std::vector<Texture> loaded_textures;

        // position and rotation
        glm::vec3 position;
        std::vector<Mesh> meshes;   // all meshes re
        std::string directory;
    
        /**
         * @brief Recursively process aiNodes within an aiScene and store the data in a Model
         * 
         * @param node node
         * @param scene scene 
         */
        void process_node(aiNode *node, const aiScene *scene);
    
        /**
         * @brief Recursively process meshes after importing
         * 
         * @param mesh imported assimp mesh
         * @param scene imported assimp scene
         * @return Model::Mesh 
         */
        Model::Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
    
        /**
         * @brief Load a model from a filepath
         * 
         * @param m model
         * @param path path to model file
         */
        friend Model import_model(const std::string &path) {
            Model model;
            Assimp::Importer importer;
            
            const aiScene *scene = importer.ReadFile(path,
                aiProcess_Triangulate |
                aiProcess_GenSmoothNormals |
                aiProcess_FlipUVs |
                aiProcess_CalcTangentSpace);
            
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << "\n";
            }

            model.directory = path.substr(0, path.find_last_of('/'));
            model.process_node(scene->mRootNode, scene);

            return model;
        }       
    };

    /**
     * @brief Load a texture from an image file
     * 
     * @param path the path to the image file
     * @return unsigned int ??
     */
    unsigned int load_texture(const char *path);

    /**
     * @brief Load a model from a filepath
     * 
     * @param m model
     * @param path path to model file
     */
    Model import_model(const std::string &path);
} // namespace xen

#endif