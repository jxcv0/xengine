#version 460 core
layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec3 norm_in;
layout (location = 2) in vec2 tex_coord_in;

out vec3 frag_pos;
out vec3 normal;
out vec2 tex_coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

/**
 * Send data to fragment shader to be loaded into G-buffer.
 */
void main() {
  vec4 world_pos = model * vec4(pos_in, 1.0);
  frag_pos = world_pos.xyz;
  tex_coord = tex_coord_in;

  // normals must be in model space
  mat3 normal_matrix = transpose(inverse(mat3(model)));
  normal = normal_matrix * norm_in;

  gl_Position = projection * view * world_pos;
}
