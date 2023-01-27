#version 460 core

out vec4 frag_col;

in vec2 tex_coord;

uniform sampler2D g_pos;
uniform sampler2D g_norm;
uniform sampler2D g_tex;

struct light {
  vec3 m_position;
  vec3 m_color;
  float m_constant;
  float m_linear;
  float m_quadratic;
};

const float AMBIENT_STRENGTH = 0.1;
uniform uint num_lights;
uniform light lights[32];

uniform vec3 view_pos;

void main() {
  // fetch data from G-Buffer
  vec3 frag_pos = texture(g_pos, tex_coord).rgb;
  vec3 g_normal = texture(g_norm, tex_coord).rgb;
  vec3 g_diffuse = texture(g_tex, tex_coord).rgb;
  float g_specular = texture(g_tex, tex_coord).a;

  vec3 lighting = g_diffuse * AMBIENT_STRENGTH;
  vec3 view_dir = normalize(view_pos - frag_pos);

  for (int i = 0; i < num_lights; i++) {
    vec3 light_dir = normalize(lights[i].m_position - frag_pos);

    /**
     * diffuse
     */
    vec3 diffuse =
        max(dot(g_normal, light_dir), 0.0) * g_diffuse * lights[i].m_color;

    /**
     * specular
     */
    vec3 half_dir = normalize(light_dir + view_dir);
    float spec_str = pow(max(dot(g_normal, half_dir), 0.0), 16.0);
    vec3 specular = lights[i].m_color * spec_str * g_specular;

    /**
     * light attenuation
     */
     float distance = length(lights[i].m_position - frag_pos);
     float attenuation =
         1.0 / (1.0 + lights[i].m_linear * distance + lights[i].m_quadratic
         * (distance * distance));

     diffuse *= attenuation;
     specular *= attenuation;
     lighting += (diffuse + specular);
  }

  frag_col = vec4(lighting, 1.0);
}
