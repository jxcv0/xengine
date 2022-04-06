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
    struct mesh {

        /**
         * @brief individial mesh vertices
         * 
         */
        struct vertex {
            glm::vec3 pos;  // vertex position in model space
            glm::vec3 norm; // normal vector of this vertex (will this lead to duplicate data?)
            glm::vec2 tex_coords;  // texture coords 
        };

        /**
         * @brief texture data
         * 
         */
        struct texture {
            unsigned int id;
            std::string type;
        };

        // opengl buffer handles
        unsigned int VAO, VBO, EBO;

        std::vector<vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<texture> textures;
    };

    /**
     * @brief Set up VAO, VBO, and EBO for a mesh
     * vertices, indices and textures must be set up first
     * 
     * @param m mesh
     */
    void setup_mesh(mesh &m);

    /**
     * @brief Draw a mesh using a shader program
     * 
     * @param m mesh
     * @param s shader program
     */
    void draw_mesh(const mesh &m, shader &s);

    /**
     * @brief 
     * 
     */
    struct model {
        std::vector<mesh::texture> loaded_textures;
        std::vector<mesh> meshes;
        std::string dir;
        bool gamma_correction;
    };

    /**
     * @brief Load a model from a filepath
     * 
     * @param path the file path to the model
     */
    model load_model(const char* path);

    /**
     * @brief Draw a model using a shader program
     * 
     * @param m mesh
     * @param s shader program
     */
    void draw_model(const shader &s);

} // namespace xen

#endif