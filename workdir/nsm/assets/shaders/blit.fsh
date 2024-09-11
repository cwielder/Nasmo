#version 460 core

layout (binding = 0) uniform sampler2D uFramebuffer;

in vec2 vTexCoords;

out vec4 oFragColor;

void main() {
    oFragColor = texture(uFramebuffer, vTexCoords);
}
