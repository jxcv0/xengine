#version 460 core
layout (location = 0) out vec3 g_pos;
layout (location = 1) out vec3 g_tex;
layout (location = 2) out vec3 g_norm;

in vec3 frag_pos;
in vec2 tex_coord;
in vec3 normal;

uniform sampler2D tex_diff;
uniform sampler2D dex_spec;

/**
 * Load data into G-Buffer.
 */
void main() {
  g_pos = frag_pos;
  g_norm = normalize(normal);
  g_tex.rgb = texture(tex_diff, tex_coord).rgb;
  g_tex.a = texture(tex_spec, tex_coord).a;
}
