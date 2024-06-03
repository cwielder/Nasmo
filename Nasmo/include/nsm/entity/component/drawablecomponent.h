#pragma once

#include <nsm/entity/entity.h>

namespace nsm {

    struct RenderInfo;

    class DrawableComponent : public EntityComponent {
    public:
        virtual void drawOpaque(const RenderInfo& renderInfo) = 0;
        virtual void drawTranslucent(const RenderInfo& renderInfo) = 0;

        void setTargetLayer(const std::string& layerName) { mTargetLayerHash = std::hash<std::string>{}(layerName); }
        [[nodiscard]] std::size_t getTargetLayerHash() const { return mTargetLayerHash; }

    private:
        std::size_t mTargetLayerHash;
    };

}
