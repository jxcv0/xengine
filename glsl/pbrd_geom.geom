#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 e_position[3];
in vec2 e_tex_coord[3];
in mat3 e_normal_matrix[3];

out vec3 position;
out mat3 normal_matrix;
out vec2 tex_coord;

void main() {
  for(int i = 0; i < 3; i++) {
    position = e_position[i];
    tex_coord = e_tex_coord[i];
    normal_matrix = e_normal_matrix[i];
    gl_Position = gl_in[i].gl_Position;
    EmitVertex();
  }
  EndPrimitive();
}
