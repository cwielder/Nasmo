#pragma once

#include <nsm/common.h>
#include <nsm/gfx/window.h>

#include <glm/vec2.hpp>

namespace nsm {

    class Event;
    class LayerStack;
    class DrawableComponent;

    class Graphics final {
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

        [[nodiscard]] f32 getTimeStep() const { return mTimeStep; }
        [[nodiscard]] LayerStack& getLayerStack() { return *mLayerStack; }

        static glm::u32vec2 getFramebufferSize();

    private:
        LayerStack* mLayerStack;
        nsm::Window mWindow;
        f32 mTimeStep, mFrameTime, mLastFrameTime;
    };

}
