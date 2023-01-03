#version 460 core
in vec2 tex_coords;
out vec2 frag_col;

uniform sampler2D text;
uniform vec3 text_color;

void main() {
  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, tex_coords).r);
  color = vec4(text_color, 1.0) * sampled;
}
