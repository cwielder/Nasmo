#version 460 core

layout (location = 0) in vec2 aPos;

out vec2 vTexCoords;

void main() {
    gl_Position = vec4(aPos, 1.0, 1.0);
    vTexCoords = aPos.xy * 0.5 + 0.5;
}
