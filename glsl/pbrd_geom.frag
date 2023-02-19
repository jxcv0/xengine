#version 460 core
layout (location = 0) out vec3 g_position;
layout (location = 1) out vec4 g_normal; // + metallic
layout (location = 2) out vec4 g_tex_diff; // + roughness

in vec3 v_position;
in vec2 v_tex_coord;
in vec3 v_tangent;
in vec3 v_bitangent;
in vec3 v_normal;

uniform sampler2D tex_diffuse;
uniform sampler2D tex_roughness;
uniform sampler2D tex_normal;
uniform sampler2D tex_metallic;

void main() {
  g_position = v_position;

  mat3 normal_matrix = mat3(v_tangent, v_bitangent, v_normal);

  vec3 normal = texture(tex_normal, v_tex_coord).rgb;
  normal = normal * 2.0 - 1.0;
  g_normal.rgb = normalize(normal_matrix * normal);
  g_normal.a = texture(tex_metallic, v_tex_coord).r;

  g_tex_diff.rgb = texture(tex_diffuse, v_tex_coord).rgb;
  g_tex_diff.a = texture(tex_roughness, v_tex_coord).r;
}
