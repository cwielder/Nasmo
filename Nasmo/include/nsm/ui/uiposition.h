#pragma once

#include <nsm/common.h>

#include <glm/vec2.hpp>

#include <string>

namespace nsm {

    class UIElement;

    class UIPosition {
    public:
        UIPosition(const UIElement* anchor, const glm::vec2& offset, const f32 z = 0.0f);
        ~UIPosition() = default;

        [[nodiscard]] glm::vec2 getXY() const;
        [[nodiscard]] f32 getZ() const;

    private:
        const UIPosition* mAnchor;
        glm::vec2 mOffset;
        f32 mZ;
    };

}
