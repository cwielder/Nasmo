#pragma once

#include <nsm/gfx/layer/Layer.h>
#include <nsm/gfx/Shader.h>
#include <nsm/gfx/RenderState.h>

namespace nsm {

    class FXAALayer final : public Layer {
    public:
        FXAALayer(const std::string& name);
        ~FXAALayer() override = default;

        void draw(const RenderInfo& renderInfo);

    private:
        ShaderProgram mShader;
    };

}
