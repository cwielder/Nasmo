#pragma once

#include <nsm/gfx/layer/Layer.h>

namespace nsm {

    class ForwardLayer : public Layer {
    public:
        ForwardLayer(const std::string& name);
        ~ForwardLayer() override = default;

        void draw(const RenderInfo& renderInfo);
    };

}
