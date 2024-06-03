#version 460 core

layout (binding = 0) uniform sampler2D tex0;

in vec2 vTexCoord;

out vec4 oFragColor;

void main() {
    oFragColor = texture(tex0, vTexCoord);
}
