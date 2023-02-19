#version 460 core
layout(triangles, equal_spacing, ccw) in;

in vec3 v_position[];
in vec3 v_tangent[];
in vec3 v_bitangent[];
in vec3 v_normal[];
in vec2 v_tex_coord[];

out vec3 e_position;
out vec3 e_tangent;
out vec3 e_bitangent;
out vec3 e_normal;
out vec2 e_tex_coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform sampler2D tex_displacement;

void main() {
  vec3 p0 = gl_TessCoord.x * v_position[0];
  vec3 p1 = gl_TessCoord.y * v_position[1];
  vec3 p2 = gl_TessCoord.z * v_position[2];
  vec3 position = p0 + p1 + p2;

  vec3 t0 = gl_TessCoord.x * v_tangent[0];
  vec3 t1 = gl_TessCoord.y * v_tangent[1];
  vec3 t2 = gl_TessCoord.z * v_tangent[2];
  vec3 tangent = t0 + t1 + t2;

  vec3 b0 = gl_TessCoord.x * v_bitangent[0];
  vec3 b1 = gl_TessCoord.y * v_bitangent[1];
  vec3 b2 = gl_TessCoord.z * v_bitangent[2];
  vec3 bitangent = b0 + b1 + b2;

  vec3 n0 = gl_TessCoord.x * v_normal[0];
  vec3 n1 = gl_TessCoord.y * v_normal[1];
  vec3 n2 = gl_TessCoord.z * v_normal[2];
  vec3 normal = normalize(n0 + n1 + n2);

  vec2 tex0 = gl_TessCoord.x * v_tex_coord[0];
  vec2 tex1 = gl_TessCoord.y * v_tex_coord[1];
  vec2 tex2 = gl_TessCoord.z * v_tex_coord[2];
  vec2 tex_coord = tex0 + tex1 + tex2;

  float disp = texture(tex_displacement, tex_coord).r;

  gl_Position = vec4(position, 1.0);

  e_position = vec3(model * vec4(position + (normal * disp * 0.15), 1.0));

  e_tangent = normalize(vec3(model * vec4(tangent, 0.0)));
  e_bitangent = normalize(vec3(model * vec4(bitangent, 0.0)));
  e_normal = normalize(vec3(model * vec4(normal, 0.0)));

  e_tex_coord = tex_coord;

  gl_Position = projection * view * model * vec4(e_position, 1.0);
}
