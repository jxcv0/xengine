#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 e_position[3];
in vec3 e_tangent[3];
in vec3 e_bitangent[3];
in vec3 e_normal[3];
in vec2 e_tex_coord[3];

out vec3 geom_position;
out vec3 geom_tangent;
out vec3 geom_bitangent;
out vec3 geom_normal;
out vec2 geom_tex_coord;

void main() {
  for(int i = 0; i < 3; i++) {
    geom_position = e_position[i];
    geom_tangent = e_tangent[i];
    geom_bitangent = e_bitangent[i];
    geom_normal = e_normal[i];
    geom_tex_coord = e_tex_coord[i];
    gl_Position = gl_in[i].gl_Position;
    EmitVertex();
  }
  EndPrimitive();
}
