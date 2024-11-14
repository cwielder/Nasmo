#pragma once

#include <nsm/common.h>
#include <nsm/ui/uirenderer.h>
#include <nsm/gfx/renderinfo.h>

#include <glm/vec2.hpp>

#include <string>

namespace nsm {

    class UIElement {
    public:
        UIElement(const glm::vec2& position)
            : mPosition(position)
            , mRenderer(&mPosition)
        { }

        virtual ~UIElement() { }

        virtual void draw(const RenderInfo& renderInfo) = 0;

    protected:
        const UIRenderer& getRenderer() const { return mRenderer; }

    protected:
        glm::vec2 mPosition;
    
    private:
        UIRenderer mRenderer;
    };

}
