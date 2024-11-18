#pragma once

#include <nsm/common.h>
#include <nsm/ui/uirenderer.h>
#include <nsm/ui/uiposition.h>
#include <nsm/gfx/renderinfo.h>

#include <glm/vec2.hpp>

#include <string>

namespace nsm {

    class UIElement {
    public:
        UIElement(const glm::vec2& position);
        UIElement(const UIElement* anchor, const glm::vec2& offset);
        virtual ~UIElement() = default;

        virtual void draw(const RenderInfo& renderInfo) = 0;

        [[nodiscard]] const UIPosition& getPosition() const { return mPosition; }

    protected:
        const UIRenderer& getRenderer() const { return mRenderer; }

    protected:
        UIPosition mPosition;
    
    private:
        UIRenderer mRenderer;
    };

}
