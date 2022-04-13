#version 460 core
out vec4 fragCol;

in vec3 col;
in vec2 texCoord;

uniform sampler2D tex;

void main() {
    fragCol = mix(texture(tex, texCoord), vec4(col, 1.0f), 0.5f);
}
