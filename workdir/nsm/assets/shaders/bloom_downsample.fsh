#version 460 core

layout (binding = 0) uniform sampler2D uTexture;
uniform vec2 uInvResolution;
uniform int uPass;

in vec2 vTexCoords;

out vec3 oFragColor;

vec3 powVec3(vec3 v, float p) {
    return vec3(pow(v.x, p), pow(v.y, p), pow(v.z, p));
}

const float invGamma = 1.0 / 2.2;
vec3 toSRGB(vec3 v) {
    return powVec3(v, invGamma);
}

float RGBToLuminance(vec3 col) {
    return dot(col, vec3(0.2126, 0.7152, 0.0722));
}

float karisAvg(vec3 col) {
    float luma = RGBToLuminance(toSRGB(col)) * 0.25;
    return 1.0 / (1.0 + luma);
}

void main() {
    vec2 srcTexelSize = uInvResolution;
    float x = srcTexelSize.x;
    float y = srcTexelSize.y;

    vec3 a = texture(uTexture, vec2(vTexCoords.x - 2*x, vTexCoords.y + 2*y)).rgb;
    vec3 b = texture(uTexture, vec2(vTexCoords.x,       vTexCoords.y + 2*y)).rgb;
    vec3 c = texture(uTexture, vec2(vTexCoords.x + 2*x, vTexCoords.y + 2*y)).rgb;

    vec3 d = texture(uTexture, vec2(vTexCoords.x - 2*x, vTexCoords.y)).rgb;
    vec3 e = texture(uTexture, vec2(vTexCoords.x,       vTexCoords.y)).rgb;
    vec3 f = texture(uTexture, vec2(vTexCoords.x + 2*x, vTexCoords.y)).rgb;

    vec3 g = texture(uTexture, vec2(vTexCoords.x - 2*x, vTexCoords.y - 2*y)).rgb;
    vec3 h = texture(uTexture, vec2(vTexCoords.x,       vTexCoords.y - 2*y)).rgb;
    vec3 i = texture(uTexture, vec2(vTexCoords.x + 2*x, vTexCoords.y - 2*y)).rgb;

    vec3 j = texture(uTexture, vec2(vTexCoords.x - x, vTexCoords.y + y)).rgb;
    vec3 k = texture(uTexture, vec2(vTexCoords.x + x, vTexCoords.y + y)).rgb;
    vec3 l = texture(uTexture, vec2(vTexCoords.x - x, vTexCoords.y - y)).rgb;
    vec3 m = texture(uTexture, vec2(vTexCoords.x + x, vTexCoords.y - y)).rgb;

    vec3 final;

    if (uPass == 1) {
        vec3 groups[5];

        groups[0] = (a + b + d + e) * (0.125 * 0.25);
        groups[1] = (b + c + e + f) * (0.125 * 0.25);
        groups[2] = (d + e + g + h) * (0.125 * 0.25);
        groups[3] = (e + f + h + i) * (0.125 * 0.25);
        groups[4] = (j + k + l + m) * (  0.5 * 0.25);
        groups[0] *= karisAvg(groups[0]);
        groups[1] *= karisAvg(groups[1]);
        groups[2] *= karisAvg(groups[2]);
        groups[3] *= karisAvg(groups[3]);
        groups[4] *= karisAvg(groups[4]);
        final = groups[0] + groups[1] + groups[2] + groups[3] + groups[4];
    } else {
        final = e * 0.125;
        final += (a + c + g + i) * 0.03125;
        final += (b + d + f + h) * 0.0625;
        final += (j + k + l + m) * 0.125;
    }

    oFragColor = final;
}
