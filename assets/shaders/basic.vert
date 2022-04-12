#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTexCoord;

out vec2 texCoord;
out vec3 Col;
in vec2 test;

void main() {
	gl_Position = vec4(aPos, 1.0f);
	texCoord = aTexCoord;
	Col = aCol;
}
