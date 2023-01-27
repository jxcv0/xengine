#version 460 core
in vec2 tex_coords;
out vec4 frag_col;

uniform sampler2D text;
uniform vec4 text_color;

void main() {
  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, tex_coords).r);
  frag_col = vec4(text_color) * sampled;
}
