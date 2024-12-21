#include <nsm/gfx/layer/uilayer.h>

#include <nsm/entity/component/drawablecomponent.h>
#include <nsm/gfx/renderinfo.h>
#include <nsm/gfx/graphics.h>

nsm::UILayer::UILayer(const std::string& name)
    : Layer(name)
    , mCamera(
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
        1.0f, -1.0f, this->getAspectRatio(), -this->getAspectRatio(),
        -1.0f, 1.0f
    )
    , mWorkBuffer()
{
    mRenderState
        .depth(false)
        .blend(RenderState::BlendFactor::SrcAlpha, RenderState::BlendFactor::OneMinusSrcAlpha, RenderState::BlendEquation::Add)
    ;

    const glm::u32vec2 size = Graphics::getFramebufferSize();

    mWorkBuffer.addTextureBuffer(Texture::Format::RGBA8, size);
    mWorkBuffer.addTextureBuffer(Texture::Format::Depth24Stencil8, size);
    mWorkBuffer.finalize();
}

void nsm::UILayer::draw(const nsm::RenderInfo& renderInfo) {
    mRenderState.apply();

    mWorkBuffer.bind();

    nsm::Framebuffer::blit(
        *renderInfo.framebuffer, mWorkBuffer,
        { 0, 0 }, renderInfo.framebuffer->getTextureBuffer(0)->getSize(),
        { 0, 0 }, mWorkBuffer.getTextureBuffer(0)->getSize(),
        static_cast<u32>(Framebuffer::Type::Color) // TODO: Get rid of this cast
    );

    nsm::RenderInfo info = renderInfo;
    info.camera = &mCamera;
    info.framebuffer = &mWorkBuffer;

    for (auto& drawable : mDrawables) {
        drawable->drawOpaque(info);
    }

    for (auto& drawable : mDrawables) {
        drawable->drawTranslucent(info);
    }

    // Blit the work buffer to the main framebuffer

    renderInfo.framebuffer->bind();

    nsm::Framebuffer::blit(
        mWorkBuffer, *renderInfo.framebuffer,
        { 0, 0 }, mWorkBuffer.getTextureBuffer(0)->getSize(),
        { 0, 0 }, renderInfo.framebuffer->getTextureBuffer(0)->getSize(),
        static_cast<u32>(Framebuffer::Type::Color) // TODO: Get rid of this cast
    );
}

void nsm::UILayer::resize(const glm::u32vec2& size) {
    mCamera.setProjection(1.0f, -1.0f, this->getAspectRatio(), -this->getAspectRatio(), -1.0f, 1.0f);

    mWorkBuffer.resize(size);
}

f32 nsm::UILayer::getAspectRatio() const {
    glm::vec2 framebufferSize = Graphics::getFramebufferSize();
    return framebufferSize.x / framebufferSize.y;
}
