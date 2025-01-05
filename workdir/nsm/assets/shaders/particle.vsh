#version 460 core

layout (location = 0) in vec2 aPos;

layout (location = 0) uniform mat4 uViewProjMtx;
layout (location = 1) uniform vec3 uScale;
layout (location = 5) uniform vec3 uPos;

out vec2 vTexCoords;

void main() {
    vec4 worldPos = vec4(uPos, 1.0);
    
    vec3 right = vec3(uViewProjMtx[0][0], uViewProjMtx[1][0], uViewProjMtx[2][0]);
    vec3 up = vec3(uViewProjMtx[0][1], uViewProjMtx[1][1], uViewProjMtx[2][1]);

    right *= uScale.x;
    up *= uScale.y;

    vec3 offset = aPos.x * right + aPos.y * up;
    vec4 billboardPos = worldPos + vec4(offset, 0.0);

    gl_Position = uViewProjMtx * billboardPos;

    vTexCoords = aPos.xy * 0.5 + 0.5;
}
