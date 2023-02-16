#version 460 core
layout (location = 0) out vec3 g_pos;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec4 g_tex_diff; // + roughness

in vec3 frag_pos;

in mat3 normal_matrix;

in vec2 tex_coord;

uniform sampler2D tex_diffuse;
uniform sampler2D tex_roughness;
uniform sampler2D tex_normal;
uniform sampler2D tex_displacement;

void main() {
  g_pos = frag_pos;
  vec3 normal = texture(tex_normal, tex_coord).rgb;
  g_normal.rgb = normalize(normal_matrix * normal);
  g_tex_diff.rgb = texture(tex_diffuse, tex_coord).rgb;
  g_tex_diff.a = texture(tex_roughness, tex_coord).r;
}
