#pragma once

#include <string>

#include <glm/vec2.hpp>

#include <nsm/gfx/OpenGL.h>

namespace nsm {

    class Window final {
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
        Window();
        ~Window();

        bool update();

        void setTitle(const std::string& title);
        void setVsync(bool enabled);

    private:
        friend class Graphics;

        void init(const WindowInfo& info);

        GLFWwindow* mHandle;
    };

}
