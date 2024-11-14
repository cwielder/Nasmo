#version 460 core

in vec2 vTexCoords;

layout (binding = 0) uniform sampler2D uTexture;

out vec4 oFragColor;

void main() {
    vec4 texColor = texture(uTexture, vTexCoords);
    if (texColor.a < 0.1) {
        discard;
    }
    oFragColor = texColor;
}
