#pragma once

#include <nsm/gfx/layer/layer.h>
#include <nsm/gfx/shader.h>
#include <nsm/gfx/renderstate.h>

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
