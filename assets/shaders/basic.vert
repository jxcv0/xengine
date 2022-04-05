#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec3 f_col;
out vec2 f_tex;

uniform mat4 global;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * global * vec4(pos, 1.0);
    f_tex = tex;
}