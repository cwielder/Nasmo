#version 460 core

in vec2 vTexCoords;
in float vLifeTime;

out vec4 oFragColor;

layout (location = 0) uniform mat4 uViewProjMtx;
layout (location = 1) uniform int uFrameCount;
layout (location = 2, binding = 0) uniform sampler2D uTexture;

void main() {
    float lifeTime = vLifeTime;
    vec2 uv = vTexCoords;
    
    float frameSize = 1.0 / uFrameCount;
    float currentFrame = floor((1.0 - lifeTime) * uFrameCount);
    float frameStart = currentFrame * frameSize;
    uv.y = frameStart + uv.y * frameSize;

    vec4 color = texture(uTexture, uv);

    if (color.a < 0.1) {
        discard;
    }

    oFragColor = color;
}
