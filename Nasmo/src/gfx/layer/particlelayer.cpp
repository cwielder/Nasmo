#include <nsm/gfx/layer/particlelayer.h>

#include <nsm/entity/component/drawablecomponent.h>
#include <nsm/entity/component/particlecomponent.h>

nsm::ParticleLayer::ParticleLayer(const std::string& name)
    : Layer(name)
{
    mRenderState
        .depth(nsm::RenderState::DepthFunction::Less, true)
        .blend(RenderState::BlendFactor::SrcAlpha, RenderState::BlendFactor::OneMinusSrcAlpha, RenderState::BlendEquation::Add)
    ;
}

void nsm::ParticleLayer::draw(const nsm::RenderInfo& renderInfo) {
    mRenderState.apply();

    for (nsm::DrawableComponent* drawable : mDrawables) {
        if (ParticleComponent* particleComponent = dynamic_cast<ParticleComponent*>(drawable)) {
            if (!particleComponent->getEmitter().hasVisual()) {
                particleComponent->getEmitter().setVisual(mParticleRenderer.getOrCreateEmitterVisual(&particleComponent->getEmitter(), particleComponent->getEmitter().getTextureAtlasPath(), particleComponent->getEmitter().getDepth()));
            }
        }
    }

    mParticleRenderer.render(renderInfo);
}
