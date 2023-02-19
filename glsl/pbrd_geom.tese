#version 460 core
layout(vertices = 4) out;

in vec3 v_frag_pos[];
in vec2 v_tex_coord[];
in mat3 v_normal_matrix[];

out vec3 tc_frag_pos[];
out vec2 tc_tex_coord[];
out mat3 tc_normal_matrix[];

void main() {
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
  tc_tex_coord[gl_InvocationID] = v_tex_coord[gl_InvocationID];

  if (gl_InvocationID == 0) {
    gl_TessLevelOuter[0] = 4;
    gl_TessLevelOuter[1] = 4;
    gl_TessLevelOuter[2] = 4;
    gl_TessLevelOuter[3] = 4;

    gl_TessLevelInner[0] = 4;
    gl_TessLevelInner[1] = 4;
  }
}
