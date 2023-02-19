#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 v_frag_pos[];
in vec2 v_tex_coord[];
in mat3 v_normal_matrix[];

out vec3 frag_pos;
out vec2 tex_coord;
out mat3 normal_matrix;

uniform sampler2D tex_displacement;

void main() {
  for(int i = 0; i < 3; i++) {
    gl_Position = gl_in[i].gl_Position;
    frag_pos = v_frag_pos[i];
    tex_coord = v_tex_coord[i];
    normal_matrix = v_normal_matrix[i];
    EmitVertex();
  }
  EndPrimitive();
}
