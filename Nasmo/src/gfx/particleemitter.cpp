#include <nsm/gfx/particleemitter.h>

#include <nsm/gfx/renderinfo.h>
#include <nsm/gfx/primitiveshape.h>
#include <nsm/entity/component/cameracomponent.h>

#include <random>
#include <limits>

#include <glm/gtc/matrix_transform.hpp>
#include <fastgltf/math.hpp>

nsm::ParticleEmitter::ParticleEmitter()
    : mParticles()
    , mRandom(std::random_device()())
    , mSSBO()
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
    , mTexture(nullptr)
{
    if (sShader == nullptr) {
        sShader = new ShaderProgram("nsm/assets/shaders/particle.vsh", "nsm/assets/shaders/particle.fsh");
    }
}

void nsm::ParticleEmitter::update(const f64 timeStep) {
    // 1. Emit new particles

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

    for (u32 i = 0; i < numParticlesToEmit; i++) {
        mParticles.push_back(Particle());
        Particle& particle = mParticles.back();

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

        particle.velocity.x += mRandom.getF32(-mInitialVelocityVariance.x, mInitialVelocityVariance.x);
        particle.velocity.y += mRandom.getF32(-mInitialVelocityVariance.y, mInitialVelocityVariance.y);
        particle.velocity.z += mRandom.getF32(-mInitialVelocityVariance.z, mInitialVelocityVariance.z);

        particle.acceleration = mAcceleration;
        particle.acceleration.x += mRandom.getF32(-mAccelerationVariance.x, mAccelerationVariance.x);
        particle.acceleration.y += mRandom.getF32(-mAccelerationVariance.y, mAccelerationVariance.y);
        particle.acceleration.z += mRandom.getF32(-mAccelerationVariance.z, mAccelerationVariance.z);

        particle.startSize = mStartSize;
        particle.startSize.x += mRandom.getF32(-mStartSizeVariance.x, mStartSizeVariance.x);
        particle.startSize.y += mRandom.getF32(-mStartSizeVariance.y, mStartSizeVariance.y);
        particle.startSize.z += mRandom.getF32(-mStartSizeVariance.z, mStartSizeVariance.z);

        particle.endSize = mEndSize;
        particle.endSize.x += mRandom.getF32(-mEndSizeVariance.x, mEndSizeVariance.x);
        particle.endSize.y += mRandom.getF32(-mEndSizeVariance.y, mEndSizeVariance.y);
        particle.endSize.z += mRandom.getF32(-mEndSizeVariance.z, mEndSizeVariance.z);

        particle.lifeTime = 0.0f;
        particle.lifeSpan = glm::max(mLifeSpan + mRandom.getF32(-mLifeSpanVariance, mLifeSpanVariance), std::numeric_limits<f32>::epsilon());
    }

    // Update existing particles
    for (Particle& particle : mParticles) {
        particle.velocity += particle.acceleration * static_cast<f32>(timeStep);
        particle.position += particle.velocity * static_cast<f32>(timeStep);

        particle.size.x = fastgltf::math::lerp(particle.startSize.x, particle.endSize.x, particle.lifeTime);
        particle.size.y = fastgltf::math::lerp(particle.startSize.y, particle.endSize.y, particle.lifeTime);
        particle.size.z = fastgltf::math::lerp(particle.startSize.z, particle.endSize.z, particle.lifeTime);

        particle.lifeTime += static_cast<f32>(timeStep) / particle.lifeSpan;
    }

    // 3. Remove dead particles
    std::erase_if(mParticles, [](const Particle& particle) {
        return particle.lifeTime >= 1.0f;
    });
}

void nsm::ParticleEmitter::render(const RenderInfo& renderInfo) {
    if (mParticles.empty()) {
        return;
    }

    PrimitiveShape::getQuadVAO().bind();

    sShader->bind();
    sShader->setMat4(0, renderInfo.camera->getViewProjection());
    sShader->setInt(4, mTexture->getFrames());

    mTexture->bind(0);

    for (u32 i = 0; i < mParticles.size(); i++) {
        const Particle& particle = mParticles[i];

        glm::vec3 effectivePosition = particle.position;

        if (mLocalSpace) {
            effectivePosition += mPosition;
        }
        
        sShader->setVec3(1, particle.size);
        sShader->setFloat(2, particle.lifeTime);
        sShader->setVec3(5, effectivePosition);

        PrimitiveShape::getQuadIBO().draw();
    }
}
