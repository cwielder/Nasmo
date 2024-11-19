#include <nsm/entity/component/uicomponent.h>

nsm::UIComponent::~UIComponent() {
    for (auto& [name, element] : mElements) {
        delete element;
    }
}

nsm::UIComponent& nsm::UIComponent::addElement(const std::string& name, UIElement* element) {
    mElements[name] = element;
    return *this;
}

[[nodiscard]] nsm::UIElement* nsm::UIComponent::getElement(const std::string& name) const {
    if (mElements.find(name) != mElements.end()) {
        return mElements.at(name);
    }

    return nullptr;
}

void nsm::UIComponent::drawOpaque(const RenderInfo& renderInfo) {
    std::vector<UIElement*> elementsSorted;
    elementsSorted.reserve(mElements.size());

    for (auto& [name, element] : mElements) {
        NSM_ASSERT(element != nullptr, "UIElement " + name + " is nullptr");

        elementsSorted.push_back(element);
    }

    std::sort(elementsSorted.begin(), elementsSorted.end(), [](UIElement* a, UIElement* b) {
        return a->getPosition().getZ() < b->getPosition().getZ();
    });

    for (auto& element : elementsSorted) {
        element->draw(renderInfo);
    }
}

void nsm::UIComponent::drawTranslucent(const RenderInfo& renderInfo) {
    // Not used for now
}
