#pragma once

#include <nsm/gfx/layer/layer.h>

namespace nsm {

    class ForwardLayer : public Layer {
    public:
        ForwardLayer(const std::string& name);
        ~ForwardLayer() override = default;

        void draw(const RenderInfo& renderInfo);
    };

}
