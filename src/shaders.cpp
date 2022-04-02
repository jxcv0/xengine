#include "shaders.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

/**
 * @brief Read a shader file
 * 
 * @param path the file path
 * @return the shader program as a string
 */
std::string ge::shaders::read_shader_file(const char* path) {
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

void ge::shaders::shader_source(GLuint shader, const std::string& shader_string) {
    const GLchar *shader_source = shader_string.c_str();
    const GLint shader_length = shader_string.length();

    glShaderSource(shader, 1, &shader_source, &shader_length);
}

void ge::shaders::load_shader(GLuint shader_obj, const char *path) {
    shader_source(shader_obj, read_shader_file(path));
}

ge::shader::shader(const char* v_path, const char* f_path) {
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
    id = glCreateProgram();
    glAttachShader(id, vert);
    glAttachShader(id, frag);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(id, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PRGM::LINKING_FAILED\n" << info_log << std::endl;
    }

    // delete shaders
    glDeleteShader(vert);
    glDeleteShader(frag);
}

void ge::shader::use() {
    glUseProgram(id);
}

void ge::shader::del() {
    glDeleteProgram(id);
}

void ge::shader::set_bool(const char *name, bool val) const {
    glUniform1i(glGetUniformLocation(id, name), (int)val); 
}

void ge::shader::set_int(const char *name, int val) const {
    glUniform1i(glGetUniformLocation(id, name), val); 
}

void ge::shader::set_float(const char *name, float val) const {
    glUniform1f(glGetUniformLocation(id, name), val); 
}