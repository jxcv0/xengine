#version 460 core
out vec4 fragCol;

in vec3 col;
in vec2 texCoord;

uniform sampler2D tex;

void main() {
    fragCol = texture(tex, texCoord);
}
