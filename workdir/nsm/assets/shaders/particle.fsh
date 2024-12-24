#version 460 core

in vec2 vTexCoords;

out vec4 oFragColor;

layout (location = 0) uniform mat4 uViewProjMtx;
layout (location = 1) uniform mat4 uMtx;
layout (location = 2) uniform float uLifeTime;
layout (location = 3, binding = 0) uniform sampler2D uTexture;
layout (location = 4) uniform int uFrameCount;

void main() {
    vec2 uv = vTexCoords;
    float frameSize = 1.0 / uFrameCount;
    float currentFrame = floor((1.0 - uLifeTime) * uFrameCount);
    float frameStart = currentFrame * frameSize;
    uv.y = frameStart + uv.y * frameSize;

    vec4 color = texture(uTexture, uv);

    if (color.a < 0.1) {
        discard;
    }

    oFragColor = color;
}
