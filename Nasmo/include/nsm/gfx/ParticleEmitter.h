#pragma once

#include <nsm/Common.h>
#include <nsm/util/RandomSource.h>
#include <nsm/gfx/Shader.h>
#include <nsm/gfx/ShaderStorage.h>
#include <nsm/gfx/AnimatedTexture.h>

#include <glm/glm.hpp>

#include <vector>
#include <memory>

namespace nsm {

    struct RenderInfo;

    class ParticleEmitter {
    public:
        ParticleEmitter();
        ~ParticleEmitter() = default;

        ParticleEmitter& setPosition(const glm::vec3& position) {
            mPosition = position;
            return *this;
        }

        glm::vec3 getPosition() const {
            return mPosition;
        }

        ParticleEmitter& setEmitRadius(const f32 emitRadius) {
            mEmitRadius = emitRadius;
            return *this;
        }

        f32 getEmitRadius() const {
            return mEmitRadius;
        }

        ParticleEmitter& setEmitRate(const f32 emitRate) {
            mEmitRate = emitRate;
            return *this;
        }

        f32 getEmitRate() const {
            return mEmitRate;
        }

        ParticleEmitter& setInitialVelocity(const glm::vec3& initialVelocity) {
            mInitialVelocity = initialVelocity;
            return *this;
        }

        glm::vec3 getInitialVelocity() const {
            return mInitialVelocity;
        }

        ParticleEmitter& setLifeSpan(const f32 lifeSpan) {
            mLifeSpan = lifeSpan;
            return *this;
        }

        f32 getLifeSpan() const {
            return mLifeSpan;
        }

        ParticleEmitter& setAcceleration(const glm::vec3& acceleration) {
            mAcceleration = acceleration;
            return *this;
        }

        glm::vec3 getAcceleration() const {
            return mAcceleration;
        }

        ParticleEmitter& setLocalSpace(const bool localSpace) {
            mLocalSpace = localSpace;
            return *this;
        }

        bool getLocalSpace() const {
            return mLocalSpace;
        }

        ParticleEmitter& setParticleLimit(const u32 particleLimit) {
            mParticleLimit = particleLimit;
            return *this;
        }

        u32 getParticleLimit() const {
            return mParticleLimit;
        }

        ParticleEmitter& setInitialVelocityVariance(const glm::vec3& variance) {
            mInitialVelocityVariance = variance;
            return *this;
        }

        ParticleEmitter& setLifeSpanVariance(const f32 variance) {
            mLifeSpanVariance = variance;
            return *this;
        }

        ParticleEmitter& setAccelerationVariance(const glm::vec3& variance) {
            mAccelerationVariance = variance;
            return *this;
        }

        glm::vec3 getInitialVelocityVariance() const {
            return mInitialVelocityVariance;
        }

        f32 getLifeSpanVariance() const {
            return mLifeSpanVariance;
        }

        glm::vec3 getAccelerationVariance() const {
            return mAccelerationVariance;
        }

        glm::vec3 getStartSize() const {
            return mStartSize;
        }

        ParticleEmitter& setStartSize(const glm::vec3& startSize) {
            mStartSize = startSize;
            return *this;
        }

        glm::vec3 getStartSizeVariance() const {
            return mStartSizeVariance;
        }

        ParticleEmitter& setStartSizeVariance(const glm::vec3& startSizeVariance) {
            mStartSizeVariance = startSizeVariance;
            return *this;
        }

        glm::vec3 getEndSize() const {
            return mEndSize;
        }

        ParticleEmitter& setEndSize(const glm::vec3& endSize) {
            mEndSize = endSize;
            return *this;
        }

        glm::vec3 getEndSizeVariance() const {
            return mEndSizeVariance;
        }

        ParticleEmitter& setEndSizeVariance(const glm::vec3& endSizeVariance) {
            mEndSizeVariance = endSizeVariance;
            return *this;
        }

        ParticleEmitter& setTexture(const std::string& path) {
            mTexture = std::make_unique<AnimatedTexture>(path);
            return *this;
        }

        ParticleEmitter& setDepth(const bool depth) {
            mDepth = depth;
            return *this;
        }

        ParticleEmitter& setEmission(const f32 emission) {
            mEmission = emission;
            return *this;
        }

        void update(const f64 timeStep);
        void render(const RenderInfo& renderInfo);

    private:
        static inline ShaderProgram* sShader = nullptr;

        struct Particle {
            union {
                glm::vec3 position;
                glm::vec3 offset;
            };
            glm::vec3 velocity;
            glm::vec3 acceleration;
            glm::vec3 startSize;
            glm::vec3 endSize;
            f32 lifeTime;
            f32 lifeSpan;
            glm::vec3 size;
        };

        std::vector<Particle> mParticles;
        RandomSource mRandom;
        f32 mParticleAccumulator;
        
        // Emission
        glm::vec3 mPosition;
        f32 mEmitRadius; // TODO: Support cubic bounding volume
        f32 mEmitRate;
        f32 mLifeSpan;
        f32 mLifeSpanVariance;
        u32 mParticleLimit;
        bool mLocalSpace = false;

        // Motion
        glm::vec3 mInitialVelocity;
        glm::vec3 mInitialVelocityVariance;
        glm::vec3 mAcceleration;
        glm::vec3 mAccelerationVariance;
        glm::vec3 mStartSize;
        glm::vec3 mStartSizeVariance;
        glm::vec3 mEndSize;
        glm::vec3 mEndSizeVariance;

        // Visuals
        std::unique_ptr<AnimatedTexture> mTexture;
        f32 mEmission;
        bool mDepth;
        // glm::vec4 mColorMultiplier;
    };

}
