#include <nsm/gfx/particlerenderer.h>

#include <nsm/gfx/renderstate.h>
#include <nsm/gfx/renderinfo.h>
#include <nsm/gfx/primitiveshape.h>
#include <nsm/gfx/particleemitter.h>
#include <nsm/entity/component/cameracomponent.h>

#include <nsm/debug/log.h>

nsm::ParticleRenderer::ParticleRenderer()
    : mEmitterVisuals()
    , mShader("nsm/assets/shaders/particle.vsh", "nsm/assets/shaders/particle.fsh")
{ }

const std::shared_ptr<nsm::EmitterVisual> nsm::ParticleRenderer::getOrCreateEmitterVisual(ParticleEmitter* customer, const std::string& textureAtlasPath, const bool depth)  {
    for (const auto& emitterVisual : mEmitterVisuals) {
        if (emitterVisual->texturePath == textureAtlasPath && emitterVisual->depth == depth) {
            emitterVisual->emitters.push_back(customer);
            return emitterVisual;
        }
    }

    std::shared_ptr<nsm::EmitterVisual> emitterVisual = std::make_shared<EmitterVisual>();
    emitterVisual->depth = depth;
    emitterVisual->texture = std::make_unique<AnimatedTexture>(textureAtlasPath);
    emitterVisual->texturePath = textureAtlasPath;
    emitterVisual->emitters.push_back(customer);

    mEmitterVisuals.push_back(emitterVisual);

    return emitterVisual;
}

void nsm::ParticleRenderer::render(const nsm::RenderInfo& renderInfo) {
    PrimitiveShape::getQuadVAO().bind();

    RenderState renderState;

    mShader.bind();
    mShader.setMat4(0, renderInfo.camera->getViewProjection());
    
    std::vector<ParticleRenderer::GPUParticle> gpuParticles;

    for (const auto& emitterVisual : mEmitterVisuals) {
        if (emitterVisual->emitters.empty()) {
            continue;
        }

        renderState.depth(RenderState::DepthFunction::LessEqual, emitterVisual->depth);
        renderState.apply(RenderState::StateBit::Depth);

        mShader.setInt(1, emitterVisual->texture->getFrames());

        emitterVisual->texture->bind(0);

        u32 particleCount = 0;
        for (const auto& emitter : emitterVisual->emitters) {
            particleCount += emitter->getGPUParticles().size();
        }

        if (particleCount == 0) {
            continue;
        }

        nsm::trace("Particle count: ", particleCount);

        gpuParticles.clear();
        gpuParticles.reserve(particleCount);
        for (const auto& emitter : emitterVisual->emitters) {
            const auto& particles = emitter->getGPUParticles();
            gpuParticles.insert(gpuParticles.end(), particles.begin(), particles.end());
        }

        emitterVisual->storage.setData(gpuParticles.size() * sizeof(ParticleRenderer::GPUParticle), gpuParticles.data(), BufferUsage::StreamDraw);
        emitterVisual->storage.bind(0);

        PrimitiveShape::getQuadIBO().drawInstanced(particleCount);
    }
}
