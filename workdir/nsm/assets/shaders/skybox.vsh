#version 460 core

layout(location = 0) in vec3 aPos;

out vec3 vTexCoords;

uniform mat4 uViewProjMtx;

void main() {
    vec4 pos = uViewProjMtx * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
    vTexCoords = vec3(aPos.x, aPos.y, -aPos.z); // Coordinate system handedness swap
}
