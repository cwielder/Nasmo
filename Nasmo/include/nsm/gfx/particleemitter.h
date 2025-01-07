#pragma once

#include <nsm/common.h>
#include <nsm/util/randomsource.h>
#include <nsm/gfx/particlerenderer.h>

#include <glm/glm.hpp>

#include <vector>
#include <memory>

namespace nsm {

    struct RenderInfo;
    class EmitterVisual;

    class ParticleEmitter {
    public:
        ParticleEmitter();
        ~ParticleEmitter();

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

        ParticleEmitter& setVisual(const std::string& textureAtlasPath, const bool depth) {
            mTextureAtlasPath = textureAtlasPath;
            mDepth = depth;
            return *this;
        }

        const std::string& getTextureAtlasPath() const {
            return mTextureAtlasPath;
        }

        bool getDepth() const {
            return mDepth;
        }

        void update(const f64 timeStep);
        
        [[nodiscard]] bool hasVisual() const { return mVisual != nullptr; }
        [[nodiscard]] const std::vector<ParticleRenderer::GPUParticle>& getGPUParticles() const { return mGPUParticles; }

    private:
        friend class ParticleLayer;

        void setVisual(std::shared_ptr<EmitterVisual> visual) { mVisual = visual; }

        static constexpr u8 cGarbageCollectInterval = 4;

    private:
        u8 mGarbageCollectCounter = 0;

        // Packed vec3
        std::vector<f32> mParticlePositions;
        std::vector<f32> mParticleVelocities;
        std::vector<f32> mParticleAccelerations;
        std::vector<f32> mParticleStartSizes;
        std::vector<f32> mParticleEndSizes;
        std::vector<f32> mParticleSizes;

        // True float
        std::vector<f32> mParticleLifeTimes;
        std::vector<f32> mParticleLifeSpans;

        std::vector<ParticleRenderer::GPUParticle> mGPUParticles;
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
        std::string mTextureAtlasPath;
        bool mDepth;
        std::shared_ptr<nsm::EmitterVisual> mVisual;
    };

}
