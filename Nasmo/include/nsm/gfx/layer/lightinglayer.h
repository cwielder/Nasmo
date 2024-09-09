#pragma once

#include <nsm/gfx/layer/layer.h>

namespace nsm {

    class LightingLayer : public Layer {
    public:
        enum class Type {
            Point,
            Directional
        };

    public:
        LightingLayer(const std::string& name, const Type type);
        ~LightingLayer() override = default;

        void draw(const RenderInfo& renderInfo);
    };

}
