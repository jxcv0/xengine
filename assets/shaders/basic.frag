#version 460 core
out vec4 frag_col;

in vec2 tex_coord;
uniform sampler2D texture_diffuse1;

void main() {
    frag_col = vec4(0.5f, 0.5f, 0.5f, 1.0f);
}