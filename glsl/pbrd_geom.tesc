#version 460 core
layout(vertices = 3) out;

#define IN_TESS 4.0
#define OUT_TESS 4.0

in vec3 v_position[];
in vec3 v_tangent[];
in vec3 v_bitangent[];
in vec3 v_normal[];
in vec2 v_tex_coord[];

out vec3 c_position[];
out vec3 c_tangent[];
out vec3 c_bitangent[];
out vec3 c_normal[];
out vec2 c_tex_coord[];

void main() {
  c_position[gl_InvocationID] = v_position[gl_InvocationID];
  c_tangent[gl_InvocationID] = v_tangent[gl_InvocationID];
  c_bitangent[gl_InvocationID] = v_bitangent[gl_InvocationID];
  c_normal[gl_InvocationID] = v_normal[gl_InvocationID];
  c_tex_coord[gl_InvocationID] = v_tex_coord[gl_InvocationID];

  if (gl_InvocationID == 0) {
    gl_TessLevelOuter[0] = OUT_TESS;
    gl_TessLevelOuter[1] = OUT_TESS;
    gl_TessLevelOuter[2] = OUT_TESS;
    gl_TessLevelInner[0] = OUT_TESS;
  }
}
