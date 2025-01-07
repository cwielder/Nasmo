#include <nsm/entity/component/particlecomponent.h>

void nsm::ParticleComponent::onUpdate(const f64 timeStep) {
    mEmitter.update(timeStep);
}

void nsm::ParticleComponent::drawOpaque(const RenderInfo& renderInfo) {

}

void nsm::ParticleComponent::drawTranslucent(const RenderInfo& renderInfo) {

}
