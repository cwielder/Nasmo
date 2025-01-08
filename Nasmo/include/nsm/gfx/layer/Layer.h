#pragma once

#include <nsm/gfx/RenderState.h>

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

        virtual void resize(const glm::u32vec2& size) { }

        void setCamera(CameraComponent* camera) { mCamera = camera; }
        [[nodiscard]] CameraComponent* getCamera() const { return mCamera; }

    protected:
        friend class Renderer;

        void pushDrawable(DrawableComponent* drawable) { mDrawables.push_back(drawable); }
        void clearDrawables() { mDrawables.clear(); }

        std::vector<DrawableComponent*> mDrawables;
        const std::string mName;
        CameraComponent* mCamera;
        RenderState mRenderState;
    };

}

