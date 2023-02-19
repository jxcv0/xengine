#version 460 core
layout (location = 0) out vec3 g_position;
layout (location = 1) out vec4 g_normal; // + metallic
layout (location = 2) out vec4 g_tex_diff; // + roughness

in vec3 e_position;
in vec2 e_tex_coord;
in vec3 e_tangent;
in vec3 e_bitangent;
in vec3 e_normal;

uniform sampler2D tex_diffuse;
uniform sampler2D tex_roughness;
uniform sampler2D tex_normal;
uniform sampler2D tex_metallic;

void main() {
  g_position = e_position;

  mat3 normal_matrix = mat3(e_tangent, e_bitangent, e_normal);

  vec3 normal = texture(tex_normal, e_tex_coord).rgb;
  normal = normal * 2.0 - 1.0;
  g_normal.rgb = normalize(normal_matrix * normal);
  g_normal.a = texture(tex_metallic, e_tex_coord).r;

  g_tex_diff.rgb = texture(tex_diffuse, e_tex_coord).rgb;
  g_tex_diff.a = texture(tex_roughness, e_tex_coord).r;
}
