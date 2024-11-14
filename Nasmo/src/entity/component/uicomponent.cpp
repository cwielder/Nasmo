#include <nsm/entity/component/uicomponent.h>

nsm::UIComponent::~UIComponent() {
    for (auto& [name, element] : mElements) {
        delete element;
    }
}

void nsm::UIComponent::drawOpaque(const RenderInfo& renderInfo) {
    for (auto& [name, element] : mElements) {
        element->draw(renderInfo);
    }
}

void nsm::UIComponent::drawTranslucent(const RenderInfo& renderInfo) {
    // Not used for now
}
