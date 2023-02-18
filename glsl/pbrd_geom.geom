#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 frag_pos_arr[];
in vec2 tex_coord_arr[];
in mat3 normal_matrix_arr[];

out vec3 frag_pos;
out vec2 tex_coord;
out mat3 normal_matrix;

void main() {
  for(int i = 0; i < 3; i++) {
    gl_Position = gl_in[i].gl_Position;
    frag_pos = frag_pos_arr[i];
    tex_coord = tex_coord_arr[i];
    normal_matrix = normal_matrix_arr[i];
    EmitVertex();
  }
  EndPrimitive();
}
