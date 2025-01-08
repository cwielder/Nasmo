#pragma once

#include <nsm/Common.h>
#include <nsm/ui/UIRenderer.h>
#include <nsm/ui/UIPosition.h>
#include <nsm/gfx/RenderInfo.h>

#include <glm/vec2.hpp>

#include <string>

namespace nsm {

    class UIElement {
    public:
        UIElement(const glm::vec2& position, const f32 z);
        UIElement(const UIElement* anchor, const glm::vec2& offset, const f32 z);
        virtual ~UIElement() = default;

        virtual void draw(const RenderInfo& renderInfo) = 0;

        [[nodiscard]] const UIPosition& getPosition() const { return mPosition; }

    protected:
        UIRenderer& getRenderer() { return mRenderer; }

    protected:
        UIPosition mPosition;
    
    private:
        UIRenderer mRenderer;
    };

}
