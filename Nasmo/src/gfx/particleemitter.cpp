#include <nsm/gfx/particleemitter.h>

#include <nsm/gfx/renderinfo.h>
#include <nsm/gfx/primitiveshape.h>
#include <nsm/entity/component/cameracomponent.h>

#include <random>

#include <glm/gtc/matrix_transform.hpp>

nsm::ParticleEmitter::ParticleEmitter()
    : mParticles()
    , mRandom(std::random_device()())
    , mPosition(0.0f)
    , mEmitRadius(0.0f)
    , mEmitRate(0.0f)
    , mInitialVelocity(0.0f)
    , mLifeSpan(0.0f)
    , mAcceleration(0.0f)
    , mLocalSpace(false)
{
    if (sShader == nullptr) {
        sShader = new ShaderProgram("nsm/assets/shaders/particle.vsh", "nsm/assets/shaders/particle.fsh");
    }
}

void nsm::ParticleEmitter::update(const f32 timeStep) {
    u32 numParticlesToEmit = static_cast<u32>(mEmitRate * timeStep);

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
        particle.lifeTime = 0.0f;
    }

    // 2. Update existing particles
    for (Particle& particle : mParticles) {
        particle.velocity += mAcceleration * timeStep; 

        particle.position += particle.velocity * timeStep;

        particle.lifeTime += timeStep / mLifeSpan;
    }

    // 3. Remove dead particles
    std::erase_if(mParticles, [](const Particle& particle) {
        return particle.lifeTime >= 1.0f;
    });
}

void nsm::ParticleEmitter::render(const RenderInfo& renderInfo) {
    PrimitiveShape::getQuadVAO().bind();

    sShader->bind();
    sShader->setMat4("uViewProjMtx", renderInfo.camera->getViewProjection());

    nsm::trace("Particle count: ", mParticles.size());

    for (const Particle& particle : mParticles) {
        glm::mat4 mtx = glm::mat4(1.0f);
        mtx = glm::translate(mtx, particle.position);
        mtx = glm::scale(mtx, glm::vec3(0.1f));

        sShader->setMat4("uModelMtx", mtx);

        PrimitiveShape::getQuadIBO().draw();
    }
}
