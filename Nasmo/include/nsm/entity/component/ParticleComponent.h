#pragma once

#include <nsm/entity/component/DrawableComponent.h>

#include <nsm/gfx/ParticleEmitter.h>

namespace nsm {

    class ParticleComponent : public DrawableComponent {
    public:
        ParticleComponent() = default;
        ~ParticleComponent() override = default;

        void onUpdate(const f64 timeStep) override;

        void drawOpaque(const RenderInfo& renderInfo) override;
        void drawTranslucent(const RenderInfo& renderInfo) override;

        ParticleEmitter& getEmitter() { return mEmitter; }

    private:
        ParticleEmitter mEmitter;
    };

}
