#pragma once

#include <nsm/gfx/layer/layer.h>

#include <nsm/gfx/particlerenderer.h>

namespace nsm {

    class ParticleLayer : public Layer {
    public:
        ParticleLayer(const std::string& name);
        ~ParticleLayer() override = default;

        void draw(const RenderInfo& renderInfo);

    private:
        ParticleRenderer mParticleRenderer;
    };

}
