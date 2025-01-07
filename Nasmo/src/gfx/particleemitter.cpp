#include <nsm/gfx/particleemitter.h>

#include <random>
#include <limits>
#include <chrono>

#include <nsm/debug/log.h>

#include <glm/gtc/matrix_transform.hpp>
#include <fastgltf/math.hpp>

#include <immintrin.h>

nsm::ParticleEmitter::ParticleEmitter()
    : mRandom(std::random_device{}())
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
    mParticlePositions.reserve(mParticleLimit);
    mParticleVelocities.reserve(mParticleLimit);
    mParticleAccelerations.reserve(mParticleLimit);
    mParticleStartSizes.reserve(mParticleLimit);
    mParticleEndSizes.reserve(mParticleLimit);
    mParticleLifeTimes.reserve(mParticleLimit);
    mParticleLifeSpans.reserve(mParticleLimit);

    mGPUParticles.reserve(mParticleLimit);
}

nsm::ParticleEmitter::~ParticleEmitter() {
    if (mVisual) {
        mVisual->removeSelf(this);
    }
}

void nsm::ParticleEmitter::update(const f64 timeStep) {
    auto start = std::chrono::high_resolution_clock::now();

    // Mark dead particles
    std::vector<std::size_t> deadIndices;
    deadIndices.reserve(mParticleLifeTimes.size());
    for (std::size_t i = 0; i < mParticleLifeTimes.size(); i++) {
        if (mParticleLifeTimes[i] >= 1.0f) {
            deadIndices.push_back(i);
        }
    }

    const auto remove_dead_single = [&deadIndices](auto& vec) {
        auto it = vec.begin();
        for (std::size_t i = 0, deadIdx = 0; i < vec.size(); i++) {
            if (deadIdx < deadIndices.size() && i == deadIndices[deadIdx]) {
                deadIdx++;
            } else {
                *it++ = std::move(vec[i]);
            }
        }
        vec.resize(vec.size() - deadIndices.size());
    };

    // remove_dead_triple treats three entries as a single entry
    const auto remove_dead_triple = [&deadIndices](auto& vec) {
        auto it = vec.begin();
        for (std::size_t i = 0, deadIdx = 0; i < vec.size(); i += 3) {
            if (deadIdx < deadIndices.size() && i == deadIndices[deadIdx] * 3) {
                deadIdx++;
            } else {
                *it++ = std::move(vec[i]);
                *it++ = std::move(vec[i + 1]);
                *it++ = std::move(vec[i + 2]);
            }
        }
        vec.resize(vec.size() - deadIndices.size() * 3);
    };

    remove_dead_triple(mParticlePositions);
    remove_dead_triple(mParticleVelocities);
    remove_dead_triple(mParticleAccelerations);
    remove_dead_triple(mParticleStartSizes);
    remove_dead_triple(mParticleEndSizes);
    remove_dead_triple(mParticleSizes);

    remove_dead_single(mParticleLifeTimes);
    remove_dead_single(mParticleLifeSpans);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    nsm::trace("Particle garbage collection took ", duration.count(), " nanoseconds");

    // Emit new particles

    f64 particlesToEmit = mEmitRate * timeStep + mParticleAccumulator;

    u32 numParticlesToEmit = static_cast<u32>(particlesToEmit);
    mParticleAccumulator = static_cast<f32>(particlesToEmit - static_cast<f64>(numParticlesToEmit));

    if (mParticlePositions.size() >= mParticleLimit) {
        numParticlesToEmit = 0;
        mParticleAccumulator = 0.0f;
    } else if (numParticlesToEmit + mParticlePositions.size() > mParticleLimit) {
        numParticlesToEmit = mParticleLimit - static_cast<u32>(mParticlePositions.size());
        mParticleAccumulator = 0.0f;
    }

    start = std::chrono::high_resolution_clock::now();

    std::vector<f32> newParticleVectors(numParticlesToEmit * 3);
    for (std::size_t i = 0; i < numParticlesToEmit; i++) {
        newParticleVectors[i * 3] = mRandom.getF32(-mEmitRadius, mEmitRadius) + (mPosition.x * !mLocalSpace);
        newParticleVectors[i * 3 + 1] = mRandom.getF32(-mEmitRadius, mEmitRadius) + (mPosition.y * !mLocalSpace);
        newParticleVectors[i * 3 + 2] = mRandom.getF32(-mEmitRadius, mEmitRadius) + (mPosition.z * !mLocalSpace);
    }
    mParticlePositions.insert(mParticlePositions.end(), newParticleVectors.begin(), newParticleVectors.end());

    for (std::size_t i = 0; i < numParticlesToEmit; i++) {
        newParticleVectors[i * 3] = mInitialVelocity.x + mRandom.getF32(-mInitialVelocityVariance.x, mInitialVelocityVariance.x);
        newParticleVectors[i * 3 + 1] = mInitialVelocity.y + mRandom.getF32(-mInitialVelocityVariance.y, mInitialVelocityVariance.y);
        newParticleVectors[i * 3 + 2] = mInitialVelocity.z + mRandom.getF32(-mInitialVelocityVariance.z, mInitialVelocityVariance.z);
    }
    mParticleVelocities.insert(mParticleVelocities.end(), newParticleVectors.begin(), newParticleVectors.end());

    for (std::size_t i = 0; i < numParticlesToEmit; i++) {
        newParticleVectors[i * 3] = mAcceleration.x + mRandom.getF32(-mAccelerationVariance.x, mAccelerationVariance.x);
        newParticleVectors[i * 3 + 1] = mAcceleration.y + mRandom.getF32(-mAccelerationVariance.y, mAccelerationVariance.y);
        newParticleVectors[i * 3 + 2] = mAcceleration.z + mRandom.getF32(-mAccelerationVariance.z, mAccelerationVariance.z);
    }
    mParticleAccelerations.insert(mParticleAccelerations.end(), newParticleVectors.begin(), newParticleVectors.end());

    for (std::size_t i = 0; i < numParticlesToEmit; i++) {
        newParticleVectors[i * 3] = mStartSize.x + mRandom.getF32(-mStartSizeVariance.x, mStartSizeVariance.x);
        newParticleVectors[i * 3 + 1] = mStartSize.y + mRandom.getF32(-mStartSizeVariance.y, mStartSizeVariance.y);
        newParticleVectors[i * 3 + 2] = mStartSize.z + mRandom.getF32(-mStartSizeVariance.z, mStartSizeVariance.z);
    }
    mParticleStartSizes.insert(mParticleStartSizes.end(), newParticleVectors.begin(), newParticleVectors.end());
    mParticleSizes.insert(mParticleSizes.end(), newParticleVectors.begin(), newParticleVectors.end());

    for (std::size_t i = 0; i < numParticlesToEmit; i++) {
        newParticleVectors[i * 3] = mEndSize.x + mRandom.getF32(-mEndSizeVariance.x, mEndSizeVariance.x);
        newParticleVectors[i * 3 + 1] = mEndSize.y + mRandom.getF32(-mEndSizeVariance.y, mEndSizeVariance.y);
        newParticleVectors[i * 3 + 2] = mEndSize.z + mRandom.getF32(-mEndSizeVariance.z, mEndSizeVariance.z);
    }
    mParticleEndSizes.insert(mParticleEndSizes.end(), newParticleVectors.begin(), newParticleVectors.end());

    std::vector<f32> newParticleFloats(numParticlesToEmit);
    for (f32& lifeTime : newParticleFloats) {
        lifeTime = 0.0f;
    }
    mParticleLifeTimes.insert(mParticleLifeTimes.end(), newParticleFloats.begin(), newParticleFloats.end());

    for (f32& lifeSpan : newParticleFloats) {
        lifeSpan = glm::max(mLifeSpan + mRandom.getF32(-mLifeSpanVariance, mLifeSpanVariance), std::numeric_limits<f32>::epsilon());
    }
    mParticleLifeSpans.insert(mParticleLifeSpans.end(), newParticleFloats.begin(), newParticleFloats.end());

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    nsm::trace("Particle emission took ", duration.count(), " nanoseconds");

    // Update existing particles
    start = std::chrono::high_resolution_clock::now();

    const __m256 timeStepVec = _mm256_set1_ps(static_cast<f32>(timeStep));

    // update velocities
    #ifdef NSM_SIMD_FALLBACK
    for (std::size_t i = 0; i < mParticleVelocities.size(); i++) {
        mParticleVelocities[i] += mParticleAccelerations[i] * static_cast<f32>(timeStep);
    }
    #else
    std::size_t i = 0;
    for (; i < mParticleVelocities.size(); i += 8) {
        const __m256 acceleration = _mm256_loadu_ps(mParticleAccelerations.data() + i);
        __m256 velocity = _mm256_loadu_ps(mParticleVelocities.data() + i);
        velocity = _mm256_fmadd_ps(acceleration, timeStepVec, velocity);
        _mm256_storeu_ps(mParticleVelocities.data() + i, velocity);
    }
    for (; i < mParticleVelocities.size(); i++) {
        mParticleVelocities[i] += mParticleAccelerations[i] * static_cast<f32>(timeStep);
    }
    #endif

    
    // update positions
    #ifdef NSM_SIMD_FALLBACK
    for (std::size_t i = 0; i < mParticlePositions.size(); i++) {
        mParticlePositions[i] += mParticleVelocities[i] * static_cast<f32>(timeStep);
    }
    #else
    i = 0;
    for (; i < mParticlePositions.size(); i += 8) {
        const __m256 velocity = _mm256_loadu_ps(mParticleVelocities.data() + i);
        __m256 position = _mm256_loadu_ps(mParticlePositions.data() + i);
        position = _mm256_fmadd_ps(velocity, timeStepVec, position);
        _mm256_storeu_ps(mParticlePositions.data() + i, position);
    }
    for (; i < mParticlePositions.size(); i++) {
        mParticlePositions[i] += mParticleVelocities[i] * static_cast<f32>(timeStep);
    }
    #endif

    // update sizes
    for (std::size_t i = 0; i < mParticleLifeTimes.size(); i++) {
        mParticleSizes[i * 3] = fastgltf::math::lerp(mParticleStartSizes[i * 3], mParticleEndSizes[i * 3], mParticleLifeTimes[i]);
        mParticleSizes[i * 3 + 1] = fastgltf::math::lerp(mParticleStartSizes[i * 3 + 1], mParticleEndSizes[i * 3 + 1], mParticleLifeTimes[i]);
        mParticleSizes[i * 3 + 2] = fastgltf::math::lerp(mParticleStartSizes[i * 3 + 2], mParticleEndSizes[i * 3 + 2], mParticleLifeTimes[i]);
    }

    // update life times
    #ifdef NSM_SIMD_FALLBACK
    for (std::size_t i = 0; i < mParticleLifeTimes.size(); i++) {
        mParticleLifeTimes[i] += static_cast<f32>(timeStep) / mParticleLifeSpans[i];
    }
    #else
    i = 0;
    for (; i < mParticleLifeTimes.size(); i += 8) {
        const __m256 lifeSpan = _mm256_loadu_ps(mParticleLifeSpans.data() + i);
        __m256 lifeTime = _mm256_loadu_ps(mParticleLifeTimes.data() + i);
        lifeTime = _mm256_fmadd_ps(timeStepVec, _mm256_rcp_ps(lifeSpan), lifeTime);
        _mm256_storeu_ps(mParticleLifeTimes.data() + i, lifeTime);
    }
    for (; i < mParticleLifeTimes.size(); i++) {
        mParticleLifeTimes[i] += static_cast<f32>(timeStep) / mParticleLifeSpans[i];
    }
    #endif

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    nsm::trace("Particle update took ", duration.count(), " nanoseconds");

    mGPUParticles.resize(mParticleLifeTimes.size());
    
    glm::vec3 offset = glm::vec3(0.0f);

    if (mLocalSpace) {
        offset += mPosition;
    }

    start = std::chrono::high_resolution_clock::now();

    std::size_t index = 0;
    for (std::size_t i = 0; i < mParticleLifeTimes.size(); i++) {
        mGPUParticles[index].position = glm::vec3(
            mParticlePositions[i * 3],
            mParticlePositions[i * 3 + 1],
            mParticlePositions[i * 3 + 2]
        ) + offset;

        mGPUParticles[index].size = glm::vec3(
            mParticleSizes[i * 3],
            mParticleSizes[i * 3 + 1],
            mParticleSizes[i * 3 + 2]
        );

        mGPUParticles[index].lifeTime = mParticleLifeTimes[i];

        index++;
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    nsm::trace("Particle GPU update took ", duration.count(), " nanoseconds");
}
