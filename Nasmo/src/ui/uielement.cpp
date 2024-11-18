#include <nsm/ui/uielement.h>

nsm::UIElement::UIElement(const glm::vec2& position)
    : mPosition(nullptr, position)
    , mRenderer(&mPosition)
{ }

nsm::UIElement::UIElement(const UIElement* anchor, const glm::vec2& offset)
    : mPosition(anchor, offset)
    , mRenderer(&mPosition)
{ }
