#pragma once

#include <nsm/common.h>
#include <nsm/gfx/window.h>
#include <nsm/app/system.h>

#include <glm/vec2.hpp>

namespace nsm {

    class Event;
    class LayerStack;
    class DrawableComponent;

    class Graphics final : public System {
    public:
        struct GraphicsInfo {
            nsm::Window::WindowInfo window;
        };
    
    public:
        Graphics(const GraphicsInfo& info);
        ~Graphics();

        [[nodiscard]] bool update();
        void onEvent(const Event* event);

        void pushDrawable(DrawableComponent* drawable);

        void transferData(Application& app, const std::vector<Entity*>& entities) override;

        [[nodiscard]] f32 getTimeStep() const { return mTimeStep; }
        [[nodiscard]] LayerStack& getLayerStack() { return *mLayerStack; }
        [[nodiscard]] f32 getFrameTime() const { return mFrameTime; }
        [[nodiscard]] nsm::Window& getWindow() { return mWindow; }

        static glm::u32vec2 getFramebufferSize();

    private:
        LayerStack* mLayerStack;
        nsm::Window mWindow;
        f32 mTimeStep, mFrameTime, mLastFrameTime;
    };

}
