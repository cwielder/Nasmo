#include <nsm/gfx/layer/layerstack.h>

#include <nsm/gfx/layer/layer.h>
#include <nsm/gfx/primitiveshape.h>
#include <nsm/entity/component/drawablecomponent.h>

nsm::LayerStack::LayerStack(const glm::u32vec2& size)
    : mLayers()
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

nsm::LayerStack::~LayerStack() {
    this->clearLayers();
}

void nsm::LayerStack::popLayer() {
    if (!mLayers.empty()) {
        delete mLayers.back().second;
        mLayers.pop_back();
    }
}

void nsm::LayerStack::removeLayer(const std::string& name) {
    const std::size_t targetHash = std::hash<std::string>{}(name);

    auto it = this->getLayerIterator(targetHash);
    NSM_ASSERT(it != mLayers.end(), "Unable to remove nonexistent layer: ", name);
    
    delete it->second;
    mLayers.erase(it);
}

void nsm::LayerStack::clearLayers() {
    for (auto& [hash, layer] : mLayers) {
        delete layer;
    }

    mLayers.clear();
}

nsm::Layer* nsm::LayerStack::getLayer(const std::size_t hash) {
    auto it = this->getLayerIterator(hash);

    NSM_ASSERT(it != mLayers.end(), "Unable to find layer with hash: ", hash);

    return it->second;
}

void nsm::LayerStack::resize(const glm::u32vec2& size) {
    mViewport.resize(size);
    mFramebuffer.resize(size);

    for (auto& [hash, layer] : mLayers) {
        layer->resize(size);
    }
}

void nsm::LayerStack::pushDrawable(DrawableComponent* drawable) {
    auto it = this->getLayerIterator(drawable->getTargetLayerHash());

    NSM_ASSERT(it != mLayers.end(), "Unable to push drawable to nonexistent layer: ", drawable->getTargetLayerHash());

    it->second->mDrawables.push_back(drawable);
}

void nsm::LayerStack::drawLayers() const {
    mViewport.apply();

    Framebuffer::getBackbuffer()->clear(glm::f32vec4{ 0.0f }, Framebuffer::Type::Color);
    Framebuffer::getBackbuffer()->clear(glm::f32vec4{ 1.0f }, Framebuffer::Type::Depth);

    mFramebuffer.bind();
    mFramebuffer.clear(glm::f32vec4{ 0.0f }, Framebuffer::Type::Color);
    mFramebuffer.clear(glm::f32vec4{ 1.0f }, Framebuffer::Type::Depth);

    for (const auto& [hash, layer] : mLayers) {
        const RenderInfo renderInfo = {
            .camera = layer->getCamera(),
            .framebuffer = &mFramebuffer,
        };

        layer->draw(renderInfo);
        layer->mDrawables.clear();
    }

    mRenderState.apply();

    Framebuffer::getBackbuffer()->bind();

    PrimitiveShape::getQuadVAO().bind();
    PrimitiveShape::getQuadIBO().bind();

    mCompositorShader.bind();
    mFramebuffer.getTextureBuffer(0)->bind(0);

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(PrimitiveShape::getQuadIBO().getCount()), GL_UNSIGNED_INT, nullptr);
}

nsm::LayerStack::LayerContainer::iterator nsm::LayerStack::getLayerIterator(const std::size_t hash) {
    return std::find_if(mLayers.begin(), mLayers.end(), [hash](const auto& pair) {
        return pair.first == hash;
    });
}
