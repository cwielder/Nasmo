#version 460 core

layout (location = 0) in vec2 aPos;

layout (location = 0) uniform mat4 uViewProjMtx;
layout (location = 1) uniform mat4 uMtx;
layout (location = 2) uniform float uLifeTime;

out vec2 vTexCoords;

void main() {
    mat4 mtx = uMtx;

    vec4 pos = vec4(aPos, 0.0, 1.0);
    gl_Position = uViewProjMtx * mtx * pos;
    vTexCoords = aPos.xy * 0.5 + 0.5;
}
