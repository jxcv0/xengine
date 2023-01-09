#version 460 core
out vec4 frag_color;

in vec3 pos;
in vec2 tex_coord;
in vec3 normal;

// lights
uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 diffuse_color;

// view position
uniform vec3 camera_pos;

// textures
uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;

void main() {
  // normal map
  normal = texture(normal_texture, tex_coord).rgb;
  normal = normalize(normal * 2.0 - 1.0);

  vec3 light_dir = normalize(light_pos - pos);

  // ambient
  vec3 ambient = light_color * vec3(texture(diffuse_texture, tex_coord));

  // diffuse
  float diffuse_strength = max(dot(normal, light_dir), 0.0);
  vec3 diffuse =
      diffuse_strength * light_color * texture(diffuse_texture, tex_coord).rgb;

  // specular
  vec3 view_dir = normalize(camera_pos - pos);
  vec3 reflect_dir = reflect(-light_dir, normal);
  float specular_strength =
      pow(max(dot(view_dir, reflect_dir), 0.0), 32); // TODO specular coeff
  vec3 specular =
      specular_strength * light_color * texture(specular_texture, tex_coord).rgb;

  vec3 result = (ambient + diffuse + specular);
  frag_color = vec4(result, 1.0);
}
