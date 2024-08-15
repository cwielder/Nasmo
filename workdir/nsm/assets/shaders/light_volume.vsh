#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 uViewProjMtx;
uniform mat4 uModelMtx;

void main() {
    gl_Position = uViewProjMtx * uModelMtx * vec4(aPos, 1.0);
}
