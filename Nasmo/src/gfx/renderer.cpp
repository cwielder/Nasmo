#include <nsm/gfx/renderer.h>

#include <nsm/gfx/layer/layer.h>
#include <nsm/gfx/primitiveshape.h>
#include <nsm/entity/component/drawablecomponent.h>
#include <nsm/entity/component/cameracomponent.h>
#include <nsm/gfx/renderpipeline.h>

nsm::Renderer::Renderer(const glm::u32vec2& size)
    : mPipeline(nullptr)
    , mFramebuffer()
    , mCompositorShader("nsm/assets/shaders/screen.vsh", "nsm/assets/shaders/blit.fsh")
    , mRenderState()
    , mViewport(size)
{
    mFramebuffer.addTextureBuffer(Texture::Format::RGB16F, size);
    mFramebuffer.addTextureBuffer(Texture::Format::Depth24Stencil8, size);
    mFramebuffer.finalize();

    mRenderState
        .blend(false)
        .cull(false)
        .depth(false)
    ;
}

nsm::Renderer::~Renderer() {
    delete mPipeline;
}

void nsm::Renderer::resize(const glm::u32vec2& size) {
    mViewport.resize(size);
    mFramebuffer.resize(size);

    NSM_ASSERT(mPipeline != nullptr, "No render pipeline set");

    mPipeline->resize(size);
}

void nsm::Renderer::pushDrawable(DrawableComponent* drawable) {
    mPipeline->getLayer(drawable->getTargetLayerHash())->pushDrawable(drawable);
}

void nsm::Renderer::applyCamera(CameraComponent* camera) {
    for (const auto& [hash, layer] : mPipeline->getLayers()) {
        if (camera->isTargetLayer(hash)) {
            layer->setCamera(camera);
        }
    }
}

void nsm::Renderer::render() {
    mViewport.apply();

    Framebuffer::getBackbuffer()->clear(glm::f32vec4{ 0.0f }, Framebuffer::Type::Color);
    Framebuffer::getBackbuffer()->clear(glm::f32vec4{ 1.0f }, Framebuffer::Type::Depth);

    mFramebuffer.bind();
    mFramebuffer.clear(glm::f32vec4{ 0.0f }, Framebuffer::Type::Color);
    mFramebuffer.clear(glm::f32vec4{ 1.0f }, Framebuffer::Type::Depth);

    mPipeline->render(&mFramebuffer);

    nsm::RenderPipeline::LayerContainer& layers = mPipeline->getLayers();
    for (auto& [hash, layer] : layers) {
        layer->clearDrawables();
    }

    mRenderState.apply();

    Framebuffer::getBackbuffer()->bind();

    mCompositorShader.bind();
    mFramebuffer.getTextureBuffer(0)->bind(0);

    // final render to screen
    PrimitiveShape::getQuadVAO().bind();
    PrimitiveShape::getQuadIBO().draw();
}
