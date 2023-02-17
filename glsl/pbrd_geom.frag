#version 460 core
layout (location = 0) out vec3 g_pos;
layout (location = 1) out vec4 g_normal; // + metallic
layout (location = 2) out vec4 g_tex_diff; // + roughness

in vec3 frag_pos;
in vec2 tex_coord;
in mat3 normal_matrix;

uniform sampler2D tex_diffuse;
uniform sampler2D tex_roughness;
uniform sampler2D tex_normal;
uniform sampler2D tex_metallic;

void main() {
  g_pos = frag_pos;

  vec3 normal = texture(tex_normal, tex_coord).rgb;
  normal = normal * 2.0 - 1.0;
  g_normal.rgb = normalize(normal_matrix * normal);
  g_normal.a = texture(tex_metallic, tex_coord).r;

  g_tex_diff.rgb = texture(tex_diffuse, tex_coord).rgb;
  g_tex_diff.a = texture(tex_roughness, tex_coord).r;
}
