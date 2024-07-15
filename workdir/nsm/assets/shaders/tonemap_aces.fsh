#version 460 core

layout (binding = 0) uniform sampler2D uFramebuffer;
uniform float uExponent;

in vec2 vTexCoords;

out vec4 FragColor;

vec3 aces(vec3 v) {
    v *= 0.6f;
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((v*(a*v+b))/(v*(c*v+d)+e), 0.0f, 1.0f);
}

void main() {
    vec4 color = texture(uFramebuffer, vTexCoords);

    vec3 cc = aces(color.rgb); // Apply ACES tonemapping
    cc = pow(cc, vec3(1.0f / uExponent)); // Apply gamma correction
    
    FragColor = vec4(cc, color.a);
}
