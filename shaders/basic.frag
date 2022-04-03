#version 460 core
out vec4 frag_colour;

uniform vec4 uni_col;

void main() {
    frag_colour = uni_col;
}