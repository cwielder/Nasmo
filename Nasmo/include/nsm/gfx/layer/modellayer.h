#pragma once

#include <nsm/gfx/layer/layer.h>

namespace nsm {

    class ModelLayer : public Layer {
    public:
        ModelLayer(const std::string& name);

        void draw(const RenderInfo& renderInfo);
    };

}
