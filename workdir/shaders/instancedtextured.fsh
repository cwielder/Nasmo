#version 460 core

layout (binding = 0) uniform sampler2D tex0;

in vec2 vTexCoord;
in vec3 vNormal;

out vec4 oFragColor;

uniform vec3 uColour;

void main() {
    oFragColor = texture(tex0, vTexCoord);
}
