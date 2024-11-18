#pragma once

#include <glm/vec2.hpp>

#include <string>

namespace nsm {

    class UIElement;

    class UIPosition {
    public:
        UIPosition(const UIElement* anchor, const glm::vec2& offset);
        ~UIPosition() = default;

        [[nodiscard]] glm::vec2 resolve() const;
        
    private:
        const UIPosition* mAnchor;
        glm::vec2 mOffset;
    };

}
