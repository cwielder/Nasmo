#include <nsm/entity/component/particlecomponent.h>

void nsm::ParticleComponent::onUpdate(const f32 timeStep) {
    mEmitter.update(timeStep);
}

void nsm::ParticleComponent::drawOpaque(const RenderInfo& renderInfo) {
    nsm::trace("Drawing opaque particles");
    mEmitter.render(renderInfo);
}

void nsm::ParticleComponent::drawTranslucent(const RenderInfo& renderInfo) {
    // TODO: Implement
}
