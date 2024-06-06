#include <nsm/gfx/window.h>

#include <nsm/common.h>
#include <nsm/gfx/opengl.h>
#include <nsm/event/events.h>
#include <nsm/app/application.h>

nsm::Window::Window()
    : mHandle(nullptr)
{ }

nsm::Window::~Window() {
    glfwDestroyWindow(mHandle);
    glfwTerminate();
}

bool nsm::Window::update() {
    glfwSwapBuffers(mHandle);
    glfwPollEvents();

    if (glfwWindowShouldClose(mHandle)) {
        return false;
    }

    return true;
}

void nsm::Window::setTitle(const std::string& title) {
    glfwSetWindowTitle(mHandle, title.c_str());
}

void nsm::Window::setViewport(const glm::u32vec2& size) {
    glViewport(0, 0, size.x, size.y);
}

void nsm::Window::init(const WindowInfo& info) {
    auto createWindow = [&](bool resizable) {
        glfwWindowHint(GLFW_RESIZABLE, resizable);
        glfwWindowHint(GLFW_DECORATED, true);

        return glfwCreateWindow(info.size.x, info.size.y, info.title.c_str(), nullptr, nullptr);
    };

    switch (info.type) {
        case WindowType::Windowed: {
            mHandle = createWindow(true);
            break;
        }

        case WindowType::StaticWindowed: {
            mHandle = createWindow(false);
            break;
        }

        case WindowType::Fullscreen: {
            glfwWindowHint(GLFW_RESIZABLE, false);
            glfwWindowHint(GLFW_DECORATED, false);

            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            mHandle = glfwCreateWindow(mode->width, mode->height, info.title.c_str(), glfwGetPrimaryMonitor(), nullptr);

            break;
        }
    }

    glfwSetWindowIconifyCallback(mHandle, [](GLFWwindow*, i32 iconified) {
        if (!iconified) {
            Application::raiseEvent(new nsm::WindowMaximizeEvent());
        }
    });

    glfwSetWindowSizeCallback(mHandle, [](GLFWwindow*, i32 width, i32 height) {
        if (width == 0 || height == 0) {
            Application::raiseEvent(new nsm::WindowMinimizeEvent());
        } else {
            Application::raiseEvent(new nsm::WindowResizeEvent(width, height));
        }
    });

    glfwSetKeyCallback(mHandle, [](GLFWwindow*, i32 key, i32 scancode, i32 action, i32 mods) {
        if (action == GLFW_PRESS) {
            Application::raiseEvent(new nsm::KeyPressEvent(key, scancode, mods));
        } else if (action == GLFW_RELEASE) {
            Application::raiseEvent(new nsm::KeyReleaseEvent(key, scancode, mods));
        } else if (action == GLFW_REPEAT) {
            Application::raiseEvent(new nsm::KeyRepeatEvent(key, scancode, mods));
        }
    });

    glfwSetMouseButtonCallback(mHandle, [](GLFWwindow*, i32 button, i32 action, i32 mods) {
        if (action == GLFW_PRESS) {
            Application::raiseEvent(new nsm::MousePressEvent(button, mods));
        } else if (action == GLFW_RELEASE) {
            Application::raiseEvent(new nsm::MouseReleaseEvent(button, mods));
        }
    });

    glfwSetCursorPosCallback(mHandle, [](GLFWwindow*, f64 x, f64 y) {
        Application::raiseEvent(new nsm::MouseMoveEvent(x, y));
    });

    glfwMakeContextCurrent(mHandle);
}

void nsm::Window::setVsync(bool enabled) {
    glfwSwapInterval(enabled);
}
