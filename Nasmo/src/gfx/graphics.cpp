#include <nsm/gfx/graphics.h>

#include <nsm/gfx/opengl.h>
#include <nsm/debug/log.h>
#include <nsm/event/events.h>

#include <glm/common.hpp>

static void glErrorCallback(GLenum, GLenum, GLuint, GLenum severity, GLsizei, const GLchar* message, const void*) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: nsm::error(message); break;
        case GL_DEBUG_SEVERITY_MEDIUM: nsm::warn(message); break;
        case GL_DEBUG_SEVERITY_LOW: nsm::info(message); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: nsm::trace(message); break;
    }
}

nsm::Graphics::Graphics(const GraphicsInfo& info)
    : mWindow()
    , mTimeStep(0.0f)
    , mFrameTime(0.0f)
    , mLastFrameTime(0.0f)
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

    mWindow.init(info.window);

    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

    #ifdef NSM_DEBUG
        glDebugMessageCallback(&glErrorCallback, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    #endif
}

nsm::Graphics::~Graphics() {
    //delete mLayerStack;
}

bool nsm::Graphics::update() {
    GLFWwindow* window = mWindow.mHandle;

    glfwPollEvents();
    glfwSwapBuffers(window);

    const f32 time = static_cast<f32>(glfwGetTime());
	mFrameTime = time - mLastFrameTime;
	mTimeStep = glm::min(mFrameTime, 0.0333f);
	mLastFrameTime = time;

    //mLayerStack->drawLayers();

    return !glfwWindowShouldClose(window);
}

void nsm::Graphics::onEvent(const Event* event) {
    if (event->getType() == nsm::EventType::WindowResize) {
        const WindowResizeEvent* e = static_cast<const WindowResizeEvent*>(event);
        mWindow.setViewport(e->getSize());
    }
}
