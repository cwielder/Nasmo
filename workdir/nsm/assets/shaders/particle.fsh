#version 460 core

in vec2 vTexCoords;

out vec4 oFragColor;

void main() {
    oFragColor = vec4(vTexCoords, 0.0, 1.0);
}
