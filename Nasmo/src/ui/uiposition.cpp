#include <nsm/ui/uiposition.h>

#include <nsm/ui/uielement.h>

nsm::UIPosition::UIPosition(const UIElement* anchor, const glm::vec2& offset, const f32 z)
    : mAnchor(anchor == nullptr ? nullptr : &anchor->getPosition())
    , mOffset(offset)
    , mZ(z)
{ }

glm::vec2 nsm::UIPosition::getXY() const {
    if (mAnchor == nullptr) {
        return mOffset;
    }

    return mAnchor->getXY() + mOffset;
}

f32 nsm::UIPosition::getZ() const {
    return mZ;
}
