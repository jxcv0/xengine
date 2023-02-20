#version 460 core
layout(triangles, equal_spacing, ccw) in;

#define DISPLACE 0.1

in vec3 c_position[];
in vec3 c_tangent[];
in vec3 c_bitangent[];
in vec3 c_normal[];
in vec2 c_tex_coord[];

out vec3 e_position;
out mat3 e_normal_matrix;
out vec2 e_tex_coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// uniform sampler2D tex_displacement;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2) {
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 +
        vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2) {
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 +
        vec3(gl_TessCoord.z) * v2;
}

void main() {
  vec2 tex0 = c_tex_coord[0];
  vec2 tex1 = c_tex_coord[1];
  vec2 tex2 = c_tex_coord[2];
  vec2 tex_coord = interpolate2D(tex0, tex1, tex2);

  // float disp = texture(tex_displacement, tex_coord).r;
  // disp = disp * 2.0 - 1.0;

  vec3 p0 = c_position[0];
  vec3 p1 = c_position[1];
  vec3 p2 = c_position[2];
  vec3 position = interpolate3D(p0, p1, p2);

  vec3 t0 = c_tangent[0];
  vec3 t1 = c_tangent[1];
  vec3 t2 = c_tangent[2];
  vec3 tangent = interpolate3D(t0, t1, t2);

  vec3 b0 = c_bitangent[0];
  vec3 b1 = c_bitangent[1];
  vec3 b2 = c_bitangent[2];
  vec3 bitangent = interpolate3D(b0, b1, b2);

  vec3 n0 = c_normal[0];
  vec3 n1 = c_normal[1];
  vec3 n2 = c_normal[2];
  vec3 normal = normalize(interpolate3D(n0, n1, n2));

  vec3 t = normalize(vec3(model * vec4(tangent, 0.0)));
  vec3 b = normalize(vec3(model * vec4(bitangent, 0.0)));
  vec3 n = normalize(vec3(model * vec4(normal, 0.0)));
  e_normal_matrix = mat3(t, b, n);

  // e_position = vec3(model * vec4(position + (normal * disp * DISPLACE), 0.0));
  e_position = position;
  e_tex_coord = tex_coord;

  gl_Position = projection * view * model * vec4(e_position, 1.0);
}
