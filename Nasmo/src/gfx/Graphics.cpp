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
#include <nsm/entity/component/SphereColliderComponent.h>
#include <nsm/entity/component/TransformComponent.h>
#include <nsm/app/Application.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <tracy/TracyOpenGL.hpp>

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

    #ifdef NSM_DEV_MODE
        TracyGpuContext;
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

    #ifdef NSM_DEV_MODE
        TracyGpuCollect;
    #endif

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

#ifdef NSM_DEV_MODE
    this->drawComponentsDebug(entities);
#endif
}

void nsm::Graphics::drawComponentsDebug(const std::vector<Entity*>& entities) {
    struct SphereDebugDrawable : public DrawableComponent {
        SphereDebugDrawable() { setTargetLayer("forward"); }

        void drawOpaque(const RenderInfo& renderInfo) { }
        void drawTranslucent(const RenderInfo& renderInfo) {
            static ShaderProgram* shader = nullptr;
            if (!shader) {
                shader = new ShaderProgram("nsm/assets/shaders/debug.vsh", "nsm/assets/shaders/debug.fsh");
            }

            glm::mat4 mtx = glm::mat4(1.0f);
            mtx = glm::translate(mtx, position);
            mtx = glm::scale(mtx, glm::vec3(scale));

            shader->bind();
            shader->setMat4("uModelMtx", mtx);
            shader->setMat4("uViewProjMtx", renderInfo.camera->getViewProjection());

            PrimitiveShape::getSphereVAO().bind();
            PrimitiveShape::getSphereIBO().draw();
        }

        glm::vec3 position;
        f32 scale;
    };

    static std::vector<std::unique_ptr<SphereDebugDrawable>> sdds;
    static std::vector<std::unique_ptr<SphereDebugDrawable>> toDelete; // defer deletion by one frame

    toDelete.clear();
    toDelete = std::move(sdds);
    sdds.clear();

    for (auto entity : entities) {
        for (auto sphereColliderComponent : entity->getComponents<nsm::SphereColliderComponent>()) {
            SphereDebugDrawable* sdd = new SphereDebugDrawable();
            sdd->position = sphereColliderComponent->getTransform()->getPosition();
            sdd->scale = sphereColliderComponent->getRadius();

            mRenderer->pushDrawable(sdd);
            sdds.push_back(std::unique_ptr<SphereDebugDrawable>(sdd));
        }
    }
}

glm::u32vec2 nsm::Graphics::getFramebufferSize() {
    glm::ivec2 size;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &size.x, &size.y);
    return size;
}
