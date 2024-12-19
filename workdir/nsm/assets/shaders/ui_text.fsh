#version 460 core

in vec2 vTexCoords;

out vec4 oFragColor;

layout (binding = 0) uniform sampler2D uTexture;
uniform vec4 uForegroundColor;
uniform vec4 uBackgroundColor;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
    const float pixelRange = 2.0; // Matches 2.0 constant in atlas gen code

    vec3 msd = texture(uTexture, vTexCoords).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPixelDistance = pixelRange * (sd - 0.5);
    float opacity = clamp(screenPixelDistance + 0.5, 0.0, 1.0);    
    oFragColor = mix(uBackgroundColor, uForegroundColor, opacity);
}
