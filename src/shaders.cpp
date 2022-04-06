#include "shaders.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/ext.hpp>

/**
 * @brief Read a shader file
 * 
 * @param path the file path
 * @return the shader program as a string
 */
std::string xen::read_shader_file(const char* path) {
    std::ifstream f_stream(path);

    if (!f_stream.is_open()) {
        std::cerr << "File not found at " << path << "." << std::endl;
        return "";
    }

    std::string str(
        (std::istreambuf_iterator<char>(f_stream)),
        (std::istreambuf_iterator<char>())
    );
    // buffer << f_stream.rdbuf();

    f_stream.close();
    return str;
}

void xen::shader_source(GLuint shader, const std::string& shader_string) {
    const GLchar *shader_source = shader_string.c_str();
    const GLint shader_length = shader_string.length();

    glShaderSource(shader, 1, &shader_source, &shader_length);
}

void xen::load_shader(GLuint shader_obj, const char *path) {
    shader_source(shader_obj, read_shader_file(path));
}

xen::Shader xen::load_shader(const char* v_path, const char* f_path) {
    
    Shader shader;

    std::string v_code;
    std::string f_code;
    std::ifstream v_file;
    std::ifstream f_file;

    v_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        std::stringstream v_stream, f_stream;

        v_file.open(v_path);
        f_file.open(f_path);

        v_stream << v_file.rdbuf();
        f_stream << f_file.rdbuf();
        
        v_file.close();
        f_file.close();

        v_code = v_stream.str();
        f_code = f_stream.str();

    } catch (std::ifstream::failure e) {
        std::cerr << "ERROR::SHADER::UNABLE_TO_READ_FILE" << std::endl;
    }

    const char* v_code_ptr = v_code.c_str();
    const char* f_code_ptr = f_code.c_str();

    unsigned int vert, frag;
    int success;
    char info_log[512];

    // compile vert
    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &v_code_ptr, NULL);
    glCompileShader(vert);

    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vert, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    // compile frag
    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &f_code_ptr, NULL);
    glCompileShader(frag);

    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(frag, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    // linking
    shader.id = glCreateProgram();
    glAttachShader(shader.id, vert);
    glAttachShader(shader.id, frag);
    glLinkProgram(shader.id);

    glGetProgramiv(shader.id, GL_LINK_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader.id, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PRGM::LINKING_FAILED\n" << info_log << std::endl;
    }

    // delete shaders
    glDeleteShader(vert);
    glDeleteShader(frag);

    return shader;
}

void xen::Shader::use() {
    glUseProgram(id);
}

void xen::Shader::del() {
    glDeleteProgram(id);
}

/**
 * @brief Uniform utility function
 * 
 * @param name shader name
 * @param val value
 */
void xen::Shader::set_bool(const char *name, bool val) const {
    glUniform1i(glGetUniformLocation(id, name), (int)val); 
}

/**
 * @brief Uniform utility function
 * 
 * @param name shader name
 * @param val value
 */
void xen::Shader::set_int(const char *name, int val) const {
    glUniform1i(glGetUniformLocation(id, name), val); 
}

/**
 * @brief Uniform utility function
 * 
 * @param name shader name
 * @param val value
 */
void xen::Shader::set_float(const char *name, float val) const {
    glUniform1f(glGetUniformLocation(id, name), val); 
}

/**
 * @brief Uniform utility function
 * 
 * @param name shader name
 * @param val value
 */
void xen::Shader::set_vec2(const char* name, const glm::vec3 &v) {
    glUniform2fv(glGetUniformLocation(id, name), 1, &v[0]);
}

/**
 * @brief Uniform utility function
 * 
 * @param name shader name
 * @param val value
 */
void xen::Shader::set_vec3(const char* name, const glm::vec3 &v) {
    glUniform3fv(glGetUniformLocation(id, name), 1, &v[0]);
}

/**
 * @brief Uniform utility function
 * 
 * @param name shader name
 * @param val value
 */
void xen::Shader::set_mat4(const char* name, const glm::mat4 &v) {
    glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(v));
}