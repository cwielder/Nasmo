#version 460 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoords;

out vec2 vTexCoords;

uniform mat4 uMtx;
uniform mat4 uProjMtx;
uniform vec4 uTexCoords;

void main() {
    vec4 pos = vec4(aPosition, 1.0, 1.0);
    gl_Position = uProjMtx * uMtx * pos;

    float minX = uTexCoords.x;
    float minY = uTexCoords.y;
    float maxX = uTexCoords.z;
    float maxY = uTexCoords.w;

    const vec2 texCoords[4] = vec2[4](
        vec2(minX, minY),
        vec2(maxX, minY),
        vec2(maxX, maxY),
        vec2(minX, maxY)
    );

    vTexCoords = texCoords[gl_VertexID];
}
