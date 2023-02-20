#version 460 core
layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec3 tangent_in;
layout (location = 2) in vec3 bitangent_in;
layout (location = 3) in vec3 normal_in;
layout (location = 4) in vec2 tex_coord_in;

out vec3 v_position;
out vec3 v_tangent;
out vec3 v_bitangent;
out vec3 v_normal;
out vec2 v_tex_coord;

void main() {
  v_position = pos_in;
  v_tangent = tangent_in;
  v_bitangent = bitangent_in;
  v_normal = normal_in;
  v_tex_coord = tex_coord_in;
}
