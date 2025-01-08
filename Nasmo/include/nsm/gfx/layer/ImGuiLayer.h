#pragma once

#include <nsm/gfx/layer/Layer.h>

namespace nsm {

    class ImGuiLayer final : public Layer {
    public:
        ImGuiLayer(const std::string& name);
        ~ImGuiLayer() override;

        void draw(const RenderInfo& renderInfo);
    };

}
