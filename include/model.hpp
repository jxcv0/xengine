#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include <glm/glm.hpp>

#include <vector>
#include <string>

#include "shaders.hpp"

// TODO dynamically allocate these at save/level loading stage
// TODO replace std::vector with the dynamically allocated memory

namespace xen {
    
    /**
     * @brief Contains mesh data imported from asset files
     * 
     */
    struct Mesh {

        /**
         * @brief individial mesh vertices
         * 
         */
        struct Vertex {
            glm::vec3 pos;  // vertex position in model space
            glm::vec3 norm; // normal vector of this vertex (will this lead to duplicate data?)
            glm::vec2 tex_coords;  // texture coords 
        };

        /**
         * @brief texture data
         * 
         */
        struct Texture {
            unsigned int id;
            std::string type;
            std::string path;
        };

        // opengl buffer handles
        unsigned int VAO, VBO, EBO;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
    };

    /**
     * @brief Set up VAO, VBO, and EBO for a mesh
     * vertices, indices and textures must be set up first
     * 
     * @param m mesh
     */
    void setup_mesh(Mesh &m);

    /**
     * @brief Draw a mesh using a shader program
     * 
     * @param m mesh
     * @param s shader program
     */
    void draw_mesh(const Mesh &m, Shader &s);

    /**
     * @brief A 3D model with mesh and textures
     * 
     */
    struct Model {
        std::vector<Mesh::Texture> loaded_textures;
        std::vector<Mesh> meshes;
        std::string dir;
        // bool gamma_correction = false;
    };

    /**
     * @brief Load a model from a filepath
     * 
     * @param m model
     * @param path path to model file
     */
    Model load_model(const std::string &path);


    unsigned int load_texture(const char *path, const std::string &dir);

    /**
     * @brief Draw a model using a shader program
     * 
     * @param m model
     * @param s shader program
     */
    void draw_model(Model &m, Shader &s);

} // namespace xen

#endif