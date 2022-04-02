#ifndef SHADERS_HPP
#define SHADERS_HPP

#include "glad.h"

#include <string>

/**
 * @brief Reading, compiling and linking shaders
 * 
 */
namespace ge {
    namespace shaders {
    
        /**
         * @brief Read a shader file
         * 
         * @param path the file path
         * @return the shader program as a string
         */
        std::string read_shader_file(const char* path);

        /**
         * @brief Assign shader source code
         * 
         * @param shader the shader onj int
         * @param shader_string the shader code
         */
        void shader_source(GLuint shader, const std::string& shader_string);

        /**
         * @brief Attach vertex shader source code to a shader object and compile
         * 
         * @param shader_obj the shader obj
         * @param path the path to the shader source code
         */
        void load_shader(GLuint shader_obj, const char *path);
    } // namespace shaders

    /**
     * @brief Class for reading, compiling and linking shaders on initialization
     * 
     */
    struct shader{

        /**
         * @brief Shader id
         * 
         */
        unsigned int id;

        /**
         * @brief Construct a new Shader object
         * 
         * @param v_path path to vertex source code
         * @param f_path path to fragment source code
         */
        shader(const char* v_path, const char* f_path);

        /**
         * @brief Use/activate the shader
         * 
         */
        void use();

        /**
         * @brief Uniform utility function
         * 
         * @param name shader name
         * @param val the value to set
         */
        void set_bool(const std::string& name, bool val) const;

        /**
         * @brief Uniform utility function
         * 
         * @param name shader name
         * @param val the value to set
         */
        void set_int(const std::string& name, int val) const;

        /**
         * @brief Uniform utility function
         * 
         * @param name shader name
         * @param val the value to set
         */
        void set_float(const std::string& name, float val) const;
    };
} // namespace shaders

#endif