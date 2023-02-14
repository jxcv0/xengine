#version 460 core
layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec2 tex_coord_in;

// G_Buffer tex coord
out vec2 tex_coord;

void main() {
  // TODO displacement
  tex_coord = tex_coord_in;
  gl_Position = vec4(pos_in, 1.0);
}
