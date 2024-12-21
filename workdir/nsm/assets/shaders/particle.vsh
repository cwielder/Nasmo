#version 460 core

layout (location = 0) in vec2 aPos;

layout (location = 0) uniform mat4 uModelMtx;
layout (location = 1) uniform mat4 uViewProjMtx;

out vec2 vTexCoords;

void main() {
    vec4 pos = vec4(aPos, 0.0, 1.0);
    gl_Position = uViewProjMtx * uModelMtx * pos;
    vTexCoords = aPos.xy * 0.5 + 0.5;
}
