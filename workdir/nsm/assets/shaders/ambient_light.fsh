#version 460 core

uniform vec3 uLightColor;

in vec2 vTexCoords;

out vec4 oFragColor;

layout (binding = 1) uniform sampler2D uAlbedoRoughness;

void main() {
    vec2 uv = vTexCoords;

    vec3 albedo = texture(uAlbedoRoughness, uv).rgb;

    oFragColor = vec4(albedo * uLightColor, 1.0);
}
