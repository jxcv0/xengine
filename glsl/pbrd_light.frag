#version 460 core

#define PI 3.141593
#define METALIC 0.0

in vec2 tex_coord;

uniform sampler2D g_pos;
uniform sampler2D g_normal;
uniform sampler2D g_tex_diff;

struct light {
  vec3 m_position;
  vec3 m_color;
  float m_constant;
  float m_linear;
  float m_quadratic;
};

const float AMBIENT_STRENGTH = 0.1;
uniform uint num_lights;
uniform light lights[8];

uniform vec3 view_pos;

out vec4 frag_col;

/**
 * Trowbridge-Reitz GGX normal distribution function.
 * Approximates the relative surface area of microfacets alighned
 * to a halfway vector.
 */
float distributionGGX(vec3 surface_normal, vec3 halfway_vector,
                      float roughness) {
  float a_squared = roughness * roughness;
  float n_dot_h = max(dot(surface_normal, halfway_vector), 0.0);
  float n_dot_h_squared = n_dot_h * n_dot_h;
  float denom = (n_dot_h_squared * (a_squared - 1.0) + 1.0);
  denom = PI * denom * denom;
  return a_squared / denom;
}

/**
 * Schlick GXX geometry function
 * Approximates the surface occlusion using roughness remapping (k)
 * and a view direction.
 */
float schlickGGX(float n_dot_v, float k) {
  float denom = n_dot_v * (1.0 - k) + k;
  return n_dot_v / denom;
}

/**
 * Smith geometry function
 * Statisticaly approximates the surface occlusion using roughness and both view
 * and light directions.
 */
float smith_geom(vec3 surface_normal, vec3 view, vec3 light, float roughness) {
  // using direct lighting for k
  float k = (roughness + 1);
  k = k * k;
  k = k / 8;

  float n_dot_v = max(dot(surface_normal, view), 0.0);
  float n_dot_l = max(dot(surface_normal, light), 0.0);
  float v = schlickGGX(n_dot_v, k);
  float l = schlickGGX(n_dot_l, k);
  return v * l;
}

/**
 * Fresnel-Schlick approximation
 * Calculated the ratio of light reflected vs refracted
 */
vec3 fresnel_schlick_approx(float cos_theta, vec3 f) {
  // f = base_color * 0.04 * metalic
  return f + (1.0 - f) * pow(1.0 - cos_theta, 5.0);
}

void main() {
  vec3 frag_pos = texture(g_pos, tex_coord).rgb;
  vec3 normal = texture(g_normal, tex_coord).rgb;

  vec3 diffuse = texture(g_tex_diff, tex_coord).rgb;
  float roughness = texture(g_tex_diff, tex_coord).a;

  vec3 f = vec3(0.04);
  f = mix(f, diffuse, METALIC);

  vec3 view_dir = normalize(view_pos - frag_pos);

  vec3 lo = vec3(0.0);

  for (int i = 0; i < num_lights; i++) {
    vec3 light_dir = normalize(lights[i].m_position - frag_pos);
    vec3 half_dir = normalize(light_dir + view_dir);
    float dist = length(lights[i].m_position - frag_pos);
    float attenuation = 1.0 /
      (1.0 + lights[i].m_linear * dist + lights[i].m_quadratic
      * (dist * dist));
    vec3 radiance = lights[i].m_color * attenuation;

    float ndf = distributionGGX(normal, half_dir, roughness);
    float geom_occ = smith_geom(normal, view_dir, light_dir, roughness);
    vec3 fresnel = fresnel_schlick_approx(max(dot(half_dir, view_dir), 0.0), f);

    vec3 ks = fresnel;
    vec3 kd = vec3(1.0) - ks;
    kd *= 1.0 - METALIC;

    float n_dot_l = max(dot(normal, light_dir), 0.0);

    vec3 num = ndf * geom_occ * fresnel;
    float denom = 4.0 * max(dot(normal, view_dir), 0.0) * n_dot_l + 0.0001;

    vec3 specular = num / denom;

    lo += (kd * diffuse / PI + specular) * radiance * n_dot_l;
  }

  vec3 ambient = vec3(0.03) * diffuse; // * ao
  vec3 color = ambient + lo;
  color = color / (color + vec3(1.0));
  color = pow(color, vec3(1.0 / 2.2));

  frag_col = vec4(color, 1.0);
}
