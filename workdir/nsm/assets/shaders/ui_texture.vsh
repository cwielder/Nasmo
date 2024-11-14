#version 460 core

layout (location = 0) in vec2 aPos;

uniform mat4 uMtx;
uniform mat4 uProj;

out vec2 vTexCoords;

void main() {
    gl_Position = vec4(aPos, 1.0, 1.0);
    gl_Position = uProj * uMtx * gl_Position;
    vTexCoords = aPos.xy * 0.5 + 0.5;
}
