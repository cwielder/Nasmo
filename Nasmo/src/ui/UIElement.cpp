#include <nsm/ui/UIElement.h>

nsm::UIElement::UIElement(const glm::vec2& position, const f32 z)
    : mPosition(nullptr, position, z)
    , mRenderer(&mPosition)
{ }

nsm::UIElement::UIElement(const UIElement* anchor, const glm::vec2& offset, const f32 z)
    : mPosition(anchor, offset, z)
    , mRenderer(&mPosition)
{ }
