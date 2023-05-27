#version 460 core
layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec3 tangent_in;
layout (location = 2) in vec3 bitangent_in;
layout (location = 3) in vec3 normal_in;
layout (location = 4) in vec2 tex_coord_in;

out vec3 position;
out vec2 tex_coord;
out mat3 normal_matrix;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  vec3 t = normalize(vec3(model * vec4(tangent_in, 0.0)));
  vec3 b = normalize(vec3(model * vec4(bitangent_in, 0.0)));
  vec3 n = normalize(vec3(model * vec4(normal_in, 0.0)));
  normal_matrix = mat3(t, b, n);
  vec4 wpos = model * vec4(pos_in, 1.0);
  position = wpos.xyz;
  
  tex_coord = tex_coord_in;

  gl_Position = projection * view * wpos;
}
