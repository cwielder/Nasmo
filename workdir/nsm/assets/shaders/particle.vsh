#version 460 core

layout (location = 0) in vec2 aPos;

layout (location = 0) uniform mat4 uViewProjMtx;

struct InstanceStruct {
    mat4 mtx;
};

layout (std430, binding = 0) buffer InstanceData {
    InstanceStruct data[];
} instanceData;

out vec2 vTexCoords;

void main() {
    mat4 mtx = instanceData.data[gl_InstanceID].mtx;

    vec4 pos = vec4(aPos, 0.0, 1.0);
    gl_Position = uViewProjMtx * mtx * pos;
    vTexCoords = aPos.xy * 0.5 + 0.5;
}
