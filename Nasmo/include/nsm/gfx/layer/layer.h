#pragma once

#include <nsm/gfx/graphicscontext.h>

#include <string>
#include <vector>

#include <glm/vec2.hpp>

namespace nsm {

    struct RenderInfo;
    class CameraComponent;
    class DrawableComponent;

    class Layer {
    public:
        Layer(const std::string& name);
        virtual ~Layer() = default;

        virtual void draw(const RenderInfo& renderInfo);
        virtual void resize(const glm::u32vec2& size) { }

        void setCamera(CameraComponent* camera) { mCamera = camera; }
        [[nodiscard]] CameraComponent* getCamera() const { return mCamera; }

    protected:
        friend class LayerStack;

        std::vector<DrawableComponent*> mDrawables;
        const std::string mName;
        CameraComponent* mCamera;
        GraphicsContext mGraphicsContext;
    };

}

