#version 460 core

layout (binding = 0) uniform sampler2D uFramebuffer;

in vec2 vTexCoords;

layout (location = 0) out vec4 oFragColor;
layout (location = 1) out vec4 oFragEmissive;

void main() {
    vec4 color = texture(uFramebuffer, vTexCoords);

    oFragColor = color;

    // if the pixel is luminous enough, add it to the emissive buffer
    float luminance = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (luminance > 0.5) {
        oFragEmissive = vec4(color.rgb, 1.0);
    } else {
        oFragEmissive = vec4(0.0, 0.0, 0.0, 0.0);
    }
}
