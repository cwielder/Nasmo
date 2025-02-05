#pragma once

#include <nsm/Common.h>
#include <nsm/gfx/Window.h>

#include <glm/vec2.hpp>

#include <vector>

namespace nsm {

    class Event;
    class Renderer;
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

        void transferData(const std::vector<Entity*>& entities);

        [[nodiscard]] Renderer& getRenderer() { return *mRenderer; }
        [[nodiscard]] Window& getWindow() { return mWindow; }

        static glm::u32vec2 getFramebufferSize();

    private:
        void drawComponentsDebug(const std::vector<Entity*>& entities);

    private:
        Renderer* mRenderer;
        Window mWindow;
    };

}
