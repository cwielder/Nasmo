#version 460 core

uniform vec2 uScreenSize;

layout (binding = 0) uniform sampler2D uNormalMetallic;
layout (binding = 1) uniform sampler2D uAlbedoRoughness;
layout (binding = 2) uniform sampler2D uDepth;

out vec4 oFragColor;

void main() {
    vec2 uv = gl_FragCoord.xy / uScreenSize;

    vec4 normalMetallic = texture(uNormalMetallic, uv);
    vec4 albedoRoughness = texture(uAlbedoRoughness, uv);

    vec3 normal = normalMetallic.xyz * 2.0 - 1.0; // ?
    vec3 albedo = albedoRoughness.rgb;
    float roughness = albedoRoughness.a;

    float depth = texture(uDepth, uv).r;

    oFragColor = vec4(albedo, 1.0);
}
