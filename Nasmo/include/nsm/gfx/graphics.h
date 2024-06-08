#pragma once

#include <nsm/common.h>
#include <nsm/gfx/window.h>

#include <glm/vec2.hpp>

#include <vector>

namespace nsm {

    class Event;
    class LayerStack;
    class DrawableComponent;
    class Application;
    class Entity;

    class Graphics final {
    public:
        struct GraphicsInfo {
            Window::WindowInfo window;
        };
    
    public:
        Graphics(const GraphicsInfo& info);
        ~Graphics();

        [[nodiscard]] bool update();
        void onEvent(const Event* event);

        void pushDrawable(DrawableComponent* drawable);

        void transferData(const std::vector<Entity*>& entities);

        [[nodiscard]] LayerStack& getLayerStack() { return *mLayerStack; }
        [[nodiscard]] Window& getWindow() { return mWindow; }

        static glm::u32vec2 getFramebufferSize();

    private:
        LayerStack* mLayerStack;
        Window mWindow;
    };

}
