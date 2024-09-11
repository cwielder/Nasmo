#version 460 core

in vec3 vTexCoords;

out vec4 oFragColor;

layout(binding = 0) uniform samplerCube uSkybox;

void main() {
    oFragColor = texture(uSkybox, vTexCoords);
}
