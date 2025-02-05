#pragma once

#include <nsm/entity/component/DrawableComponent.h>

#include <nsm/ui/UIElement.h>

namespace nsm {

    class UIComponent : public DrawableComponent {
    public:
        UIComponent() = default;
        ~UIComponent() override;

        UIComponent* addElement(const std::string& name, UIElement* element);
        
        [[nodiscard]] UIElement* getElement(const std::string& name) const;

        void drawOpaque(const RenderInfo& renderInfo) override;
        void drawTranslucent(const RenderInfo& renderInfo) override;

    private:
        std::map<std::string /* name */, UIElement*> mElements;
    };

}
