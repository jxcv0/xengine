#version 460 core
out vec4 out_col;

in vec3 f_col;
in vec2 f_tex;

uniform sampler2D, out_tex;

void main() {
    out_col = texture(out_tex, f_tex);
}