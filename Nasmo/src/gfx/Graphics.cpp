#include <nsm/gfx/Graphics.h>

#include <nsm/gfx/OpenGL.h>
#include <nsm/debug/Log.h>
#include <nsm/event/Events.h>
#include <nsm/gfx/Texture2D.h>
#include <nsm/gfx/Renderer.h>
#include <nsm/gfx/layer/Layer.h>
#include <nsm/gfx/PrimitiveShape.h>
#include <nsm/gfx/Material.h>
#include <nsm/entity/component/DrawableComponent.h>
#include <nsm/entity/component/CameraComponent.h>
#include <nsm/entity/component/ModelComponent.h>
#include <nsm/app/Application.h>

#include <glm/common.hpp>

static void glErrorCallback(GLenum, GLenum, GLuint, GLenum severity, GLsizei, const GLchar* message, const void*) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: NSM_ASSERT(false, message); break;
        case GL_DEBUG_SEVERITY_MEDIUM: nsm::warn(message); break;
        case GL_DEBUG_SEVERITY_LOW: nsm::info(message); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: nsm::trace(message); break;
    }
}

nsm::Graphics::Graphics(const GraphicsInfo& info)
    : mWindow()
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

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    PrimitiveShape::init();

    mRenderer = new Renderer(info.window.size);
}

nsm::Graphics::~Graphics() {
    delete mRenderer;

    ModelComponent::clearModels();
    Texture2D::clearCache();
    Material::clearCache();
    PrimitiveShape::destroy();
}

bool nsm::Graphics::update() {
    GLFWwindow* window = mWindow.mHandle;

    glfwPollEvents();
    glfwSwapBuffers(window);

    mRenderer->render();

    return !glfwWindowShouldClose(window);
}

void nsm::Graphics::onEvent(const Event* event) {
    if (event->getType() == nsm::EventType::WindowResize) {
        const WindowResizeEvent* e = static_cast<const WindowResizeEvent*>(event);
        mRenderer->resize(e->getSize());
    }
}

void nsm::Graphics::transferData(const std::vector<Entity*>& entities) {    
    for (auto entity : entities) {        
        for (auto drawableComponent : entity->getComponents<nsm::DrawableComponent>()) {
            mRenderer->pushDrawable(drawableComponent);
        }
    
        for (auto cameraComponent : entity->getComponents<nsm::CameraComponent>()) {
            mRenderer->applyCamera(cameraComponent);
        }
    }
}

glm::u32vec2 nsm::Graphics::getFramebufferSize() {
    glm::ivec2 size;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &size.x, &size.y);
    return size;
}
