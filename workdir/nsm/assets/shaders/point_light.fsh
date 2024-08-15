#version 460 core

uniform vec2 uScreenSize;
uniform mat4 uViewProjMtx;
uniform vec3 uCamPos;
uniform vec3 uLightPos;
uniform vec3 uLightColor;

layout (binding = 0) uniform sampler2D uNormalMetallic;
layout (binding = 1) uniform sampler2D uAlbedoRoughness;
layout (binding = 2) uniform sampler2D uDepth;

out vec4 oFragColor;

const float PI = 3.14159265359;

float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float geometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 BRDF(vec3 position, vec3 normal, vec3 albedo, float roughness, float metallic) {
    vec3 V = normalize(uCamPos - position);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 L = normalize(uLightPos - position);
    vec3 H = normalize(V + L);
    float distance = length(uLightPos - position);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = uLightColor * attenuation;

    float NDF = distributionGGX(normal, H, roughness);
    float G = geometrySmith(normal, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, V), 0.0) * max(dot(normal, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    float NdotL = max(dot(normal, L), 0.0);

    return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 getViewPositionFromDepth(vec2 uv, float depth) {
    float z = depth * 2.0 - 1.0;
    vec4 clipSpacePosition = vec4(uv * 2.0 - 1.0, z, 1.0);

    vec4 viewSpacePosition = inverse(uViewProjMtx) * clipSpacePosition;
    viewSpacePosition /= viewSpacePosition.w;

    return viewSpacePosition.xyz;
}

void main() {
    vec2 uv = gl_FragCoord.xy / uScreenSize;

    float depth = texture(uDepth, uv).r;

    if (depth == 1.0) {
        oFragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    vec4 normalMetallic = texture(uNormalMetallic, uv);
    vec4 albedoRoughness = texture(uAlbedoRoughness, uv);
    
    vec3 position = getViewPositionFromDepth(uv, depth);
    vec3 normal = normalize(normalMetallic.rgb);
    vec3 albedo = albedoRoughness.rgb;
    float roughness = albedoRoughness.a;
    float metallic = normalMetallic.w;

    vec3 color = BRDF(position, normal, albedo, roughness, metallic);

    oFragColor = vec4(color, 1.0);
}
