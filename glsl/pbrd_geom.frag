#version 460 core
layout (location = 0) out vec3 g_pos;
layout (location = 1) out vec3 g_tangent;
layout (location = 2) out vec3 g_bitangent;
layout (location = 3) out vec3 g_normal;
layout (location = 4) out vec4 g_tex_diff; // roughness
layout (location = 5) out vec4 g_tex_norm; // displacement

in vec3 frag_pos;

in vec3 tangent;
in vec3 bitangent;
in vec3 normal;

in vec2 tex_coord;

uniform sampler2D tex_diffuse;
uniform sampler2D tex_roughness;
uniform sampler2D tex_normal;
uniform sampler2D tex_displacement;

/**
 * Load everything to gbuffer
 */
void main() {
  g_pos = frag_pos;

  g_tangent = tangent;
  g_bitangent = bitangent;
  g_normal = normal;

  g_tex_diff.rgb = texture(tex_diffuse, tex_coord).rgb;
  g_tex_diff.a = texture(tex_roughness, tex_coord).r;

  g_tex_norm.rgb = texture(tex_normal, tex_coord).rgb;
  g_tex_norm.a = texture(tex_displacement, tex_coord).r;
}
