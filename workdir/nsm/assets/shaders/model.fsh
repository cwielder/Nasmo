#version 460 core

layout (binding = 0) uniform sampler2D tex0;
layout (binding = 1) uniform sampler2D tex1;
layout (binding = 2) uniform sampler2D tex2;

uniform int uAlbedoTexturePresent;
uniform int uMetallicRoughnessTexturePresent;
uniform vec4 uAlbedoFactor;
uniform float uMetallicFactor;
uniform float uRoughnessFactor;
uniform int uNormalTexturePresent;

in vec2 vTexCoord;
in mat3 vTBN;
in vec3 vPosition;
in vec3 vNormal;

layout (location = 0) out vec4 oNormalMetallic;
layout (location = 1) out vec4 oAlbedoRoughness;

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

    vec3 normal;
    if (uNormalTexturePresent == 1) {
        if (uAlbedoTexturePresent == 1 && uMetallicRoughnessTexturePresent == 1) {
            normal = texture(tex2, vTexCoord).xyz * 2.0 - 1.0;
        } else if (uAlbedoTexturePresent == 1 || uMetallicRoughnessTexturePresent == 1) {
            normal = texture(tex1, vTexCoord).xyz * 2.0 - 1.0;
        } else {
            normal = texture(tex0, vTexCoord).xyz * 2.0 - 1.0;
        }
    } else {
        normal = vNormal;
    }

    vec2 uv = vTexCoord; // apply parallax mapping here

    if (uv.x > 1.0 || uv.y > 1.0 || uv.x < 0.0 || uv.y < 0.0) {
        discard;
    }

    oNormalMetallic.rgb = normalize(vTBN * normal);
    oNormalMetallic.a = metallic;
    oAlbedoRoughness.rgb = albedo.rgb;
    oAlbedoRoughness.a = roughness;
}
