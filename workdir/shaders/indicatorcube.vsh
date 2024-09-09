#version 460 core

uniform mat4 uModelMtx;
uniform mat4 uViewProjMtx;

layout(location = 0) in vec3 aPosition;

void main() {
    gl_Position = uViewProjMtx * uModelMtx * vec4(aPosition, 1.0);
}
