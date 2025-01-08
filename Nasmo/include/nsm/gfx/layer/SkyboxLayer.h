#pragma once

#include <nsm/gfx/layer/Layer.h>

namespace nsm {

    class SkyboxLayer : public Layer {
    public:
        SkyboxLayer(const std::string& name);

        void draw(const RenderInfo& renderInfo);
    };

}
