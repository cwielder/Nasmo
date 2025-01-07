#version 460 core

layout (location = 0) in vec2 aPos;

layout (location = 0) uniform mat4 uViewProjMtx;

layout (std430) struct GPUParticle {
    vec3 size;
    float padding;
    vec3 position;
    float lifeTime;
};

layout (std430, binding = 0) buffer ParticleData {
    GPUParticle particles[];
};

out vec2 vTexCoords;
out float vLifeTime;

void main() {
    vec3 size = particles[gl_InstanceID].size;
    vec3 position = particles[gl_InstanceID].position;
    vLifeTime = particles[gl_InstanceID].lifeTime;

    vec4 worldPos = vec4(position, 1.0);
    
    vec3 right = vec3(uViewProjMtx[0][0], uViewProjMtx[1][0], uViewProjMtx[2][0]);
    vec3 up = vec3(uViewProjMtx[0][1], uViewProjMtx[1][1], uViewProjMtx[2][1]);

    right *= size.x;
    up *= size.y;

    vec3 offset = aPos.x * right + aPos.y * up;
    vec4 billboardPos = worldPos + vec4(offset, 0.0);

    gl_Position = uViewProjMtx * billboardPos;

    vTexCoords = aPos.xy * 0.5 + 0.5;
}
