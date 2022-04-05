#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include <glm/glm.hpp>

#include <vector>

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
            char type[64];
        };

        // opengl buffer handles
        unsigned int VAO_, VBO_, EBO_;

        std::vector<vertex> vertices_;
        std::vector<unsigned int> indices_;
        std::vector<texture> textures_;
    };

    /**
     * @brief Set up VAO, VBO, and EBO for a mesh
     * 
     * @param m mesh
     */
    void setup_mesh(mesh &m);

    /**
     * @brief Draw a mesh using a shader program
     * 
     * @param m mesh
     * @param s the shader program
     */
    void draw_mesh(const mesh &m, xen::shader &s);

    /**
     * @brief 
     * 
     */
    class model {

    public:

        /**
         * @brief Load a model from a filepath
         * 
         * @param path the file path to the model
         */
        void load(const char* path);
    };
} // namespace xen

#endif