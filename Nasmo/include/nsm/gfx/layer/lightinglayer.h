#pragma once

#include <nsm/gfx/layer/layer.h>

namespace nsm {

    class LightingLayer : public Layer {
    public:
        LightingLayer(const std::string& name);
        ~LightingLayer() override = default;

        void draw(const RenderInfo& renderInfo);
    };

}
