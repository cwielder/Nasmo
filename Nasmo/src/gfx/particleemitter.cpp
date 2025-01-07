#include <nsm/gfx/particleemitter.h>

#include <random>
#include <limits>
#include <chrono>

#include <nsm/debug/log.h>

#include <glm/gtc/matrix_transform.hpp>
#include <fastgltf/math.hpp>

nsm::ParticleEmitter::ParticleEmitter()
    : mParticles()
    , mRandom(std::random_device{}())
    , mParticleAccumulator(0.0f)
    , mPosition(0.0f)
    , mEmitRadius(0.0f)
    , mEmitRate(0.0f)
    , mInitialVelocity(0.0f)
    , mInitialVelocityVariance(0.0f)
    , mLifeSpan(0.0f)
    , mLifeSpanVariance(0.0f)
    , mAcceleration(0.0f)
    , mAccelerationVariance(0.0f)
    , mLocalSpace(false)
    , mParticleLimit(1000)
    , mVisual(nullptr)
{
    mParticles.reserve(mParticleLimit);
    mGPUParticles.reserve(mParticleLimit);
}

nsm::ParticleEmitter::~ParticleEmitter() {
    mParticles.clear();
    mGPUParticles.clear();

    if (mVisual) {
        mVisual->removeSelf(this);
    }
}

void nsm::ParticleEmitter::update(const f64 timeStep) {
    // Remove dead particles
    std::erase_if(mParticles, [](const Particle& particle) {
        return particle.lifeTime >= 1.0f;
    });

void nsm::ParticleEmitter::emitParticles(const f64 timeStep) {
    // Emit new particles
    f64 particlesToEmit = mEmitRate * timeStep + mParticleAccumulator;

    u32 numParticlesToEmit = static_cast<u32>(particlesToEmit);
    mParticleAccumulator = static_cast<f32>(particlesToEmit - static_cast<f64>(numParticlesToEmit));

    if (mParticles.size() >= mParticleLimit) {
        numParticlesToEmit = 0;
        mParticleAccumulator = 0.0f;
    } else if (numParticlesToEmit + mParticles.size() > mParticleLimit) {
        numParticlesToEmit = mParticleLimit - static_cast<u32>(mParticles.size());
        mParticleAccumulator = 0.0f;
    }

    std::vector<Particle> newParticles(numParticlesToEmit);
    for (Particle& particle : newParticles) {
        if (mLocalSpace) {
            particle.offset = glm::vec3(
                mRandom.getF32(-mEmitRadius, mEmitRadius),
                mRandom.getF32(-mEmitRadius, mEmitRadius),
                mRandom.getF32(-mEmitRadius, mEmitRadius)
            );
        } else { // World-space
            particle.position = mPosition + glm::vec3(
                mRandom.getF32(-mEmitRadius, mEmitRadius),
                mRandom.getF32(-mEmitRadius, mEmitRadius),
                mRandom.getF32(-mEmitRadius, mEmitRadius)
            );
        }

        particle.velocity = mInitialVelocity;
        particle.velocity += glm::vec3(
            mRandom.getF32(-mInitialVelocityVariance.x, mInitialVelocityVariance.x),
            mRandom.getF32(-mInitialVelocityVariance.y, mInitialVelocityVariance.y),
            mRandom.getF32(-mInitialVelocityVariance.z, mInitialVelocityVariance.z)
        );

        particle.acceleration = mAcceleration;
        particle.acceleration += glm::vec3(
            mRandom.getF32(-mAccelerationVariance.x, mAccelerationVariance.x),
            mRandom.getF32(-mAccelerationVariance.y, mAccelerationVariance.y),
            mRandom.getF32(-mAccelerationVariance.z, mAccelerationVariance.z)
        );

        particle.startSize = mStartSize;
        particle.startSize += glm::vec3(
            mRandom.getF32(-mStartSizeVariance.x, mStartSizeVariance.x),
            mRandom.getF32(-mStartSizeVariance.y, mStartSizeVariance.y),
            mRandom.getF32(-mStartSizeVariance.z, mStartSizeVariance.z)
        );

        particle.endSize = mEndSize;
        particle.endSize += glm::vec3(
            mRandom.getF32(-mEndSizeVariance.x, mEndSizeVariance.x),
            mRandom.getF32(-mEndSizeVariance.y, mEndSizeVariance.y),
            mRandom.getF32(-mEndSizeVariance.z, mEndSizeVariance.z)
        );

        particle.lifeTime = 0.0f;
        particle.lifeSpan = glm::max(mLifeSpan + mRandom.getF32(-mLifeSpanVariance, mLifeSpanVariance), std::numeric_limits<f32>::epsilon());
    }

    mParticles.insert(mParticles.end(), newParticles.begin(), newParticles.end());

    // Update existing particles
    for (Particle& particle : mParticles) {
        particle.velocity += particle.acceleration * static_cast<f32>(timeStep);
        particle.position += particle.velocity * static_cast<f32>(timeStep);

        particle.size.x = fastgltf::math::lerp(particle.startSize.x, particle.endSize.x, particle.lifeTime);
        particle.size.y = fastgltf::math::lerp(particle.startSize.y, particle.endSize.y, particle.lifeTime);
        particle.size.z = fastgltf::math::lerp(particle.startSize.z, particle.endSize.z, particle.lifeTime);

        particle.lifeTime += static_cast<f32>(timeStep) / particle.lifeSpan;
    }

    mGPUParticles.resize(mParticles.size());
    for (std::int_fast16_t i = 0; i < static_cast<std::int_fast16_t>(mParticles.size()); i++) {
        const Particle& particle = mParticles[i];

        glm::vec3 effectivePosition = particle.position;

        if (mLocalSpace) {
            effectivePosition += mPosition;
        }
        
        mGPUParticles[i].size = particle.size;
        mGPUParticles[i].position = effectivePosition;
        mGPUParticles[i].lifeTime = particle.lifeTime;
    }
}
