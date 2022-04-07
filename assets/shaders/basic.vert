#version 460 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_norm;
layout (location = 2) in vec3 a_tex_coord;

out vec2 tex_coord;

uniform mat4 global;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * global * vec4(a_pos, 1.0);
}