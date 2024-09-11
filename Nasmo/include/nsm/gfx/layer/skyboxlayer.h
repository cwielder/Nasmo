#pragma once

#include <nsm/gfx/layer/layer.h>

namespace nsm {

    class SkyboxLayer : public Layer {
    public:
        SkyboxLayer(const std::string& name);

        void draw(const RenderInfo& renderInfo);
    };

}
