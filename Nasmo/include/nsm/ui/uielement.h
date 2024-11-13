#pragma once

#include <nsm/common.h>
#include <nsm/ui/uirenderer.h>

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

        virtual void draw() = 0;
        virtual void update(const f32 timeStep) = 0;

    protected:
        const UIRenderer& getRenderer() const { return mRenderer; }

    protected:
        glm::vec2 mPosition;
    
    private:
        UIRenderer mRenderer;
    };

}
