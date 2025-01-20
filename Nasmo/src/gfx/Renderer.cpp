#include <nsm/gfx/Renderer.h>

#include <nsm/gfx/layer/Layer.h>
#include <nsm/gfx/PrimitiveShape.h>
#include <nsm/entity/component/DrawableComponent.h>
#include <nsm/entity/component/CameraComponent.h>
#include <nsm/gfx/pipeline/RenderPipeline.h>

nsm::Renderer::Renderer(const glm::u32vec2& size)
    : mPipeline(nullptr)
    , mFramebuffer()
    , mCompositorShader("nsm/assets/shaders/screen.vsh", "nsm/assets/shaders/blit.fsh")
    , mRenderState()
    , mViewport(size)
{
    mFramebuffer.addTextureBuffer(Texture::Format::RGB8, size);
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
    // Prepare framebuffers
    mViewport.apply();

    Framebuffer::getBackbuffer()->clear(glm::f32vec4{ 0.0f }, Framebuffer::Type::Color);
    Framebuffer::getBackbuffer()->clear(glm::f32vec4{ 1.0f }, Framebuffer::Type::Depth);

    mFramebuffer.bind();
    mFramebuffer.clear(glm::f32vec4{ 0.0f }, Framebuffer::Type::Color);
    mFramebuffer.clear(glm::f32vec4{ 1.0f }, Framebuffer::Type::Depth);

    // Run the render pipeline
    mPipeline->render(&mFramebuffer);

    // Clean up
    nsm::RenderPipeline::LayerContainer& layers = mPipeline->getLayers();
    for (auto& [hash, layer] : layers) {
        layer->clearDrawables();
    }

    // Final render to screen
    mRenderState.apply();

    Framebuffer::getBackbuffer()->bind();

    mCompositorShader.bind();
    mFramebuffer.getTextureBuffer(0)->bind(0);

    PrimitiveShape::getQuadVAO().bind();
    PrimitiveShape::getQuadIBO().draw();
}
