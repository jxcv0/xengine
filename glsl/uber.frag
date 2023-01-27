#version 460 core
out vec4 frag_color;

in vec3 pos;
in vec2 tex_coord;
in vec3 normal;

// lights
struct light {
  vec3 m_position;
  vec3 m_color;
  float m_constant;
  float m_linear;
  float m_quadratic;
};

uniform light l;

// view position
uniform vec3 camera_pos;

// textures
uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;

void main() {
  // normal map
  vec3 norm = texture(normal_texture, tex_coord).rgb;
  norm = normalize(normal * 2.0 - 1.0);

  // lighting
  vec3 light_dir = normalize(l.m_position - pos);
  float light_dist = length(l.m_position - pos);
  float light_att =
      1.0 /
      (l.m_constant + l.m_linear * light_dist + l.m_quadratic *
      (light_dist * light_dist));

  // ambient
  vec3 ambient = l.m_color * vec3(texture(diffuse_texture, tex_coord));

  // diffuse
  float diffuse_strength = max(dot(norm, light_dir), 0.0);
  vec3 diffuse =
      diffuse_strength * l.m_color * texture(diffuse_texture, tex_coord).rgb;

  // specular
  vec3 view_dir = normalize(camera_pos - pos);
  vec3 reflect_dir = reflect(-light_dir, norm);
  float specular_strength =
      pow(max(dot(view_dir, reflect_dir), 0.0), 32); // TODO specular coeff
  vec3 specular =
      specular_strength * l.m_color * texture(specular_texture, tex_coord).rgb;

  vec3 result = (ambient + diffuse + specular) * light_att;
  frag_color = vec4(result, 1.0);
}

