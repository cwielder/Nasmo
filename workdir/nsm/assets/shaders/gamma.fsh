#version 460 core

layout (binding = 0) uniform sampler2D uFramebuffer;
uniform float uExponent;

in vec2 vTexCoords;

out vec4 FragColor;

void main() {
    vec4 color = texture(uFramebuffer, vTexCoords);
    vec3 correctedColor = pow(color.rgb, vec3(1.0 / uExponent));
    FragColor = vec4(correctedColor, color.a);
}
