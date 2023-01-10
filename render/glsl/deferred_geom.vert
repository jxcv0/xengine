#version 460 core
layout (location = 0) out vec3 pos_in;
layout (location = 1) out vec3 tex_in;
layout (location = 2) out vec3 norm_in;

out vec3 frag_pos;
out vec2 tex_coord;
out vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

/**
 * Send data to fragment shader to be loaded into G-buffer.
 */
void main() {
  vec4 world_pos = model * vec4(pos_in, 1.0);
  frag_pos = world_pos.xyz;
  tex_coord = tex_in;

  // normals must be in model space
  mat3 normal_matrix = transpose(inverse(mat3(model)));
  normal = normal_matrix * normal;

  gl_Position = projection * view * world_pos
}
