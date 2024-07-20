#version 460 core

layout (binding = 0) uniform sampler2D tex0;
layout (binding = 1) uniform sampler2D tex1;

uniform int uAlbedoTexturePresent;
uniform int uMetallicRoughnessTexturePresent;
uniform vec4 uAlbedoFactor;
uniform float uMetallicFactor;
uniform float uRoughnessFactor;

in vec2 vTexCoord;
in vec3 vNormal;

out vec4 oFragColor;

void main() {
    vec4 albedo;

    if (uAlbedoTexturePresent == 1) {
        albedo = texture(tex0, vTexCoord);
    } else {
        albedo = uAlbedoFactor;
    }

    float metallic;
    float roughness;

    if (uMetallicRoughnessTexturePresent == 1) {
        if (uAlbedoTexturePresent == 1) {
            vec4 metallicRoughness = texture(tex1, vTexCoord);
            metallic = metallicRoughness.b;
            roughness = metallicRoughness.g;
        } else {
            vec4 metallicRoughness = texture(tex0, vTexCoord);
            metallic = metallicRoughness.b;
            roughness = metallicRoughness.g;
        }
    } else {
        metallic = uMetallicFactor;
        roughness = uRoughnessFactor;
    }

    oFragColor = vec4(albedo.rgb, 1.0);
}
