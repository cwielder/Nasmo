#version 460 core

layout (binding = 0) uniform sampler2D uTexture;

in vec2 vTexCoords;

out vec3 oFragColor;

void main() {
    oFragColor = texture(uTexture, vec2(vTexCoords.x, vTexCoords.y)).rgb;
}
