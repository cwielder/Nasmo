#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 uModelMtx;
uniform mat4 uViewProj;

void main() {
    gl_Position = uViewProj * uModelMtx * vec4(aPos, 1.0);
}
