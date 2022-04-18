#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec3 fragPos;
out vec2 texCoord;
out vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	fragPos = vec3(model * vec4(aPos, 1.0f));
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	texCoord = aTexCoord;
	normal = aNormal;
}
