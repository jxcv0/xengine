#version 460 core
layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec3 normal_in;
layout (location = 2) in vec2 tex_coord_in;

out vec2 tex_coord;
out vec3 normal;
out vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() { 
  tex_coord = tex_coord_in;
  normal = normalize(normal_in);
  pos = vec3(view * model * vec4(pos_in, 1.0));

  gl_Position = projection * view * model * vec4(pos_in, 1.0f);
}
