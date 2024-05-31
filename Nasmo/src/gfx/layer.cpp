#include <nsm/gfx/layer.h>

#include <nsm/entity/component/drawablecomponent.h>
#include <nsm/gfx/primitiveshape.h>

nsm::Layer::Layer(const std::string& name)
    : mDrawables()
    , mName(name)
    , mCamera(nullptr)
    , mGraphicsContext()
{
    mGraphicsContext
        .depth(GraphicsContext::DepthFunction::LessEqual, true)
    ;
}

void nsm::Layer::draw(const nsm::RenderInfo& renderInfo) {
    mGraphicsContext.apply();

    for (auto& drawable : mDrawables) {
        drawable->draw(renderInfo);
    }
}

nsm::LayerStack::LayerStack(const glm::u32vec2& size)
    : mLayers()
    , mFramebuffer(size)
    , mCompositorShader("nsm/assets/shaders/compositor.vsh", "nsm/assets/shaders/compositor.fsh")
    , mGraphicsContext()
{
    mFramebuffer.addTextureBuffer(Texture::Format::RGBA8); //? Is this the right format? check for HDR
    mFramebuffer.finalize();

    mGraphicsContext
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

    if (it != mLayers.end()) {
        delete it->second;
        mLayers.erase(it);
    } else {
        nsm::warn("Unable to remove nonexistent layer: ", name);
    }
}

void nsm::LayerStack::clearLayers() {
    for (auto& [hash, layer] : mLayers) {
        delete layer;
    }

    mLayers.clear();
}

nsm::Layer* nsm::LayerStack::getLayer(const std::size_t hash) {
    auto it = this->getLayerIterator(hash);

    if (it != mLayers.end()) {
        return it->second;
    }

    return nullptr;
}

void nsm::LayerStack::resize(const glm::u32vec2& size) {
    glViewport(0, 0, size.x, size.y);

    mFramebuffer.resize(size);

    for (auto& [hash, layer] : mLayers) {
        layer->resize(size);
    }
}

void nsm::LayerStack::pushDrawable(DrawableComponent* drawable, const std::size_t layerHash) {
    auto it = this->getLayerIterator(layerHash);

    if (it != mLayers.end()) {
        it->second->mDrawables.push_back(drawable);
    } else {
        nsm::warn("Unable to push drawable to nonexistent layer: ", layerHash);
    }
}

void nsm::LayerStack::drawLayers() const {
    Framebuffer::getBackbuffer()->clear(glm::f32vec4{ 0.0f }, Framebuffer::Type::Color);
    Framebuffer::getBackbuffer()->clear(glm::f32vec4{ 1.0f }, Framebuffer::Type::Depth);

    mFramebuffer.bind();
    mFramebuffer.clear(glm::f32vec4{ 1.0f }, Framebuffer::Type::Color);
    mFramebuffer.clear(glm::f32vec4{ 1.0f }, Framebuffer::Type::Depth);

    for (const auto& [hash, layer] : mLayers) {
        const RenderInfo renderInfo = {
            .camera = layer->getCamera(),
            .framebuffer = &mFramebuffer,
        };

        layer->draw(renderInfo);
        layer->mDrawables.clear();
    }

    mGraphicsContext.apply();

    Framebuffer::getBackbuffer()->bind();

    PrimitiveShape::getQuadVAO().bind();
    PrimitiveShape::getQuadIBO().bind();

    mCompositorShader.bind();
    mFramebuffer.getTextureBuffer(0)->bind(0);

    glDrawElements(GL_TRIANGLES, PrimitiveShape::getQuadIBO().getCount(), GL_UNSIGNED_INT, nullptr);
}

nsm::LayerStack::LayerContainer::iterator nsm::LayerStack::getLayerIterator(const std::size_t hash) {
    return std::find_if(mLayers.begin(), mLayers.end(), [hash](const auto& pair) {
        return pair.first == hash;
    });
}
