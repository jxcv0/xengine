#version 460 core
layout (location = 0) in vec3 pos_in;
layout (location = 2) in vec2 tex_coord_in;

// G-Buffer tex coord
out vec2 tex_coord;

void main() {
  tex_coord = tex_coord_in;
  gl_Position = vec4(pos_in, 1.0);
}
