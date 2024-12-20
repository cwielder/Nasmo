#pragma once

#include <nsm/entity/component/drawablecomponent.h>

#include <nsm/gfx/particleemitter.h>

namespace nsm {

    class ParticleComponent : public DrawableComponent {
    public:
        ParticleComponent() = default;
        ~ParticleComponent() override = default;

        void onUpdate(const f32 timeStep) override;

        void drawOpaque(const RenderInfo& renderInfo) override;
        void drawTranslucent(const RenderInfo& renderInfo) override;

        ParticleEmitter& getEmitter() { return mEmitter; }

    private:
        ParticleEmitter mEmitter;
    };

}
