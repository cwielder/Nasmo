#include <nsm/gfx/window.h>

#include <nsm/gfx/opengl.h>

nsm::Window::Window(const WindowInfo& info)
    : mHandle(nullptr)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef NSM_DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    #else
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
    #endif

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

    glfwMakeContextCurrent(mHandle);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

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
