#version 460 core
layout (location = 0) in vec3 pos_in;

layout (location = 1) in vec3 tangent_in;
layout (location = 2) in vec3 bitangent_in;
layout (location = 3) in vec3 normal_in;

layout (location = 4) in vec2 tex_coord_in;

out vec3 frag_pos;

out vec3 tangent;
out vec3 bitangent;
out vec3 normal;

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

  tangent = normalize(vec3(projection * view * model * vec4(tangent_in, 0.0)));
  bitangent = normalize(vec3(projection * view * model * vec4(bitangent_in, 0.0)));
  normal = normalize(vec3(projection * view * model * vec4(normal_in, 0.0)));

  gl_Position = projection * view * world_pos;
}
