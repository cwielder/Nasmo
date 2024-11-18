#include <nsm/ui/uiposition.h>

#include <nsm/ui/uielement.h>

nsm::UIPosition::UIPosition(const UIElement* anchor, const glm::vec2& offset)
    : mAnchor(anchor == nullptr ? nullptr : &anchor->getPosition())
    , mOffset(offset)
{ }

glm::vec2 nsm::UIPosition::resolve() const {
    if (mAnchor == nullptr) {
        return mOffset;
    }

    return mAnchor->resolve() + mOffset;
}
