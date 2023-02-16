#version 460 core
layout (location = 0) in vec3 pos_in;

layout (location = 1) in vec3 tangent_in;
layout (location = 2) in vec3 bitangent_in;
layout (location = 3) in vec3 normal_in;

layout (location = 4) in vec2 tex_coord_in;

out vec3 frag_pos;

out mat3 normal_matrix;

out vec2 tex_coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

/**
 * Create normal matrix
 */
void main() {
  vec4 world_pos = model * vec4(pos_in, 1.0);
  frag_pos = world_pos.xyz;
  tex_coord = tex_coord_in;

  vec3 t = normalize(vec3(projection * view * model * vec4(tangent_in, 0.0)));
  vec3 b = normalize(vec3(projection * view * model * vec4(bitangent_in, 0.0)));
  vec3 n = normalize(vec3(projection * view * model * vec4(normal_in, 0.0)));
  normal_matrix = mat3(t, b, n);

  gl_Position = projection * view * world_pos;
}
