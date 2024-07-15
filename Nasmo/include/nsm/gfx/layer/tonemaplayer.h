#pragma once

#include <nsm/gfx/layer/layer.h>
#include <nsm/gfx/shader.h>
#include <nsm/gfx/renderstate.h>

namespace nsm {

    class TonemapLayer final : public Layer {
    public:
        TonemapLayer(const std::string& name, const f32 exponent = 2.2f);
        ~TonemapLayer() override = default;

        void draw(const RenderInfo& renderInfo) override;

        void setExponent(const f32 exponent) { mExponent = exponent; }
        [[nodiscard]] f32 getExponent() const { return mExponent; }

    private:
        f32 mExponent;
        ShaderProgram mShader;
        RenderState mRenderState;
    };

}
