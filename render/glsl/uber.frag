#version 460 core
out vec4 frag_color;

in vec3 pos;
in vec2 tex_coord;
in vec3 normal;

uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 diffuse_color;

uniform sampler2D diffuse_texture;

float ambient_strength = 0.1;

void main() {
  vec3 ambient = ambient_strength * light_color;
  vec3 light_dir = normalize(light_pos - pos);

  float diff = max(dot(normal, light_dir), 0.0);
  vec3 diffuse = diff * light_color * texture(diffuse_texture, tex_coord).rgb;

  vec3 result = (ambient + diffuse) * diffuse_color;
  frag_color = vec4(result, 1.0);
}
