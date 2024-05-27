#pragma once

#include <string>

#include <glm/vec2.hpp>

#include <nsm/gfx/opengl.h>

namespace nsm {

    class Window {
    public:
        enum class WindowType {
            Windowed,
            StaticWindowed,
            Fullscreen,

            Count
        };

        struct WindowInfo {
            std::string title;
            glm::u32vec2 size;
            nsm::Window::WindowType type;
        } window;

    public:
        Window(const WindowInfo& info);
        ~Window();

        bool update();

        void setTitle(const std::string& title);
        void setViewport(const glm::u32vec2& size);

    private:
        GLFWwindow* mHandle;
    };

}
