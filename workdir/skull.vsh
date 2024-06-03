#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec3 aNormal;

out vec2 vTexCoord;

uniform mat4 uViewProjMtx;

layout (std430, binding = 0) buffer InstanceData {
    mat4 uModelMtx[];
} instanceData;

void main() {
    vTexCoord = aTexCoord;
    gl_Position = uViewProjMtx * instanceData.uModelMtx[gl_InstanceID] * vec4(aPos, 1.0);
}
