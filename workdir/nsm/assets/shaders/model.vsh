#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec3 aNormal;

out vec2 vTexCoord;
out vec3 vPosition;
out vec3 vNormal;

uniform mat4 uViewProjMtx;
uniform mat4 uNodeTransform;

struct InstanceStruct {
    mat4 transform;
};

layout (std430, binding = 0) buffer InstanceData {
    InstanceStruct data[];
} instanceData;

void main() {
    mat4 modelMatrix = instanceData.data[gl_InstanceID].transform * uNodeTransform;

    vTexCoord = aTexCoord;
    vPosition = (modelMatrix * vec4(aPos, 1.0)).xyz;
    vNormal = (modelMatrix * vec4(aNormal, 0.0)).xyz;

    gl_Position = uViewProjMtx * modelMatrix * vec4(aPos, 1.0);
}
