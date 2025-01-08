#pragma once

#include <nsm/gfx/layer/Layer.h>
#include <nsm/gfx/Shader.h>
#include <nsm/gfx/RenderState.h>

namespace nsm {

    class TonemapLayer final : public Layer {
    public:
        TonemapLayer(const std::string& name, const f32 exponent = 2.2f);
        ~TonemapLayer() override = default;

        void draw(const RenderInfo& renderInfo);

        void setExponent(const f32 exponent) { mExponent = exponent; }
        [[nodiscard]] f32 getExponent() const { return mExponent; }

    private:
        f32 mExponent;
        ShaderProgram mShader;
    };

}
