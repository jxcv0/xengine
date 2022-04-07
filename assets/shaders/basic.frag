#version 460 core
out vec4 frag_col;

in vec2 tex_coord;

uniform sampler2D texture_diffuse1;

void main() {
    frag_col = texture(texture_diffuse1, tex_coord);
}