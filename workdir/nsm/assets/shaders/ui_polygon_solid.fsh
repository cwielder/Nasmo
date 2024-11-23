#version 460 core

in vec2 vTexCoords;

layout (location = 2) uniform vec4 uColor;

out vec4 oFragColor;

void main() {
    oFragColor = uColor;
}
