#version 460 core
out vec4 fragCol;

in vec3 fragPos;
in vec2 texCoord;
in vec3 normal;
in mat3 TBN;

// each renderable object only has one material
uniform sampler2D textureDiffuse;
uniform sampler2D textureSpecular;
uniform sampler2D textureNormal;
uniform float shininess;

uniform vec3 viewPosition;

struct Light
{
	vec3 position;
	vec3 colour;

	// attenuation constants
	float constant;
	float linear;
	float quadratic;
};
uniform Light light;

// calculate on omnidirectional light
vec3 calculateLight(Light light, vec3 normal, vec3 viewDir)
{
	
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic + (distance * distance));

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
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = spec * light.colour * texture(textureSpecular, texCoord).rgb;

	return (ambient + diffuse + specular) * attenuation;
}

void main()
{
	vec3 viewDir = normalize(viewPosition - fragPos);
	vec3 normal = texture(textureNormal, texCoord).rgb;
	normal = (normal * 2.0 - 1.0);
	normal = normalize(TBN * normal);

	vec3 result = calculateLight(light, normal, viewDir);
	fragCol = vec4(result, 1.0);
}
