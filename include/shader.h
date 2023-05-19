#ifndef SHADER_H
#define SHADER_H

#include <stdint.h>

#include "glad.h"
#include "lin.h"

/**
 * @brief Uniform integer utility function.
 *
 * @param uniform_name The name of the uniform.
 * @param value The value to set the uniform to.
 */
void shader_set_uniform_1i(GLuint id, const char *uniform_name, int32_t value);

/**
 * @brief Uniform unsigned integer utility function.
 *
 * @param uniform_name The name of the uniform.
 * @param value The value to set the uniform to.
 */
void shader_set_uniform_1ui(GLuint id, const char *uniform_name,
                            uint32_t value);

/**
 * @brief Uniform float utility function.
 *
 * @param uniform_name The name of the uniform.
 * @param value The value to set the uniform to.
 */
void shader_set_uniform_1f(GLuint id, const char *uniform_name, float value);

/**
 * @brief Uniform vec2 float utility function.
 *
 * @param uniform_name The name of the uniform.
 * @param value The value to set the uniform to.
 */
void shader_set_uniform_2fv(GLuint id, const char *uniform_name,
                            float value[2]);

/**
 * @brief Uniform vec3 float utility function.
 *
 * @param uniform_name The name of the uniform.
 * @param value The value to set the uniform to.
 */
void shader_set_uniform_3fv(GLuint id, const char *uniform_name,
                            float value[3]);

/**
 * @brief Uniform vec4 float utility function.
 *
 * @param uniform_name The name of the uniform.
 * @param value The value to set the uniform to.
 */
void shader_set_uniform_4fv(GLuint id, const char *uniform_name,
                            float value[4]);

/**
 * @brief Uniform mat4 float utility function.
 *
 * @param uniform_name The name of the uniform.
 * @param value The value to set the uniform to.
 */
void shader_set_uniform_m4fv(GLuint shader, const char *uniform_name,
                             float value[4][4]);

/**
 * @brief Check the compile status of a shader and exit on error.
 *
 * @param id The id of the shader.
 */
void check_compile(GLuint id);

/**
 * @brief Check the link status of a shader program and exit on error.
 *
 * @param id The id of the program.
 */
void check_link(GLuint id);

/**
 * @brief Load a shader from a file.
 *
 * @param vert_path Path to the vertex shader.
 * @param frag_path Path to the fragment shader.
 * @return A new Shader object.
 */
GLuint load_shader_vf(const char *vert_path, const char *frag_path);

/**
 * @brief Load a shader from a file.
 *
 * @param vert_path Path to the vertex shader.
 * @param frag_path Path to the fragment shader.
 * @param geom_path Path to an optional geometry shader.
 * @return A new Shader object.
 */
GLuint load_shader_vfg(const char *vert_path, const char *frag_path,
                       const char *geom_path);

#endif  // SHADER_H
