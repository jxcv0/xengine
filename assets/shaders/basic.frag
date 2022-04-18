#version 460 core
out vec4 fragCol;

in vec3 fragPos;
in vec2 texCoord;
in vec3 normal;

// each renderable object only has one material
uniform sampler2D textureDiffuse;
uniform sampler2D textureSpecular;
uniform sampler2D textureNormal;
uniform float shininess;

uniform vec3 viewPosition;

// could be made into an array of all light positions in "scene"?
struct Light
{
	vec3 position;
	vec3 colour;
};
uniform Light light;	// array of lights with entry for every light that casts to a visible surface

void main()
{
	// ambient
	float ambientStrength = 0.1;	// get this from where?
	vec3 ambient = ambientStrength * light.colour;

	// extra normal normals
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);

	// diffuse
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.colour * texture(textureDiffuse, texCoord).rgb;

	// specular
	vec3 viewDir = normalize(viewPosition - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = spec * light.colour * texture(textureSpecular, texCoord).rgb;

	vec3 result = (ambient + diffuse + specular);
	fragCol = vec4(result, 1.0);
}
