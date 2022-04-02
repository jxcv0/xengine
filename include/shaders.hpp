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
     * @brief Class for reading, compiling and linking shaders on initialization.
     * This class manages a handle to the shader program and member functions call on the global gl
     * functions.
     * 
     * 
     * TODO - make singleton shader manager in future?
     * or create an instance in each thing that needs its own shader?
     * will know more after i understand more about memory managment and allocation models for ge
     * ALSO - thread safety for multiple ge::shader instances
     * 
     */
    struct shader{

        /**
         * @brief Shader id
         * 
         */
        unsigned int id;

        /**
         * @brief Compile and link a vertex and fragment shader into a shader program and
         * get a handle to this program
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
         * @brief Dealocate shader program resources
         * 
         */
        void del();
        
        /**
         * @brief Uniform utility function
         * 
         * @param name shader name
         * @param val the value to set
         */
        void set_bool(const char *name, bool val) const;

        /**
         * @brief Uniform utility function
         * 
         * @param name shader name
         * @param val the value to set
         */
        void set_int(const char *name, int val) const;

        /**
         * @brief Uniform utility function
         * 
         * @param name shader name
         * @param val the value to set
         */
        void set_float(const char *name, float val) const;
    };
} // namespace shaders

#endif