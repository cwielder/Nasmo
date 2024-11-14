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
{
    mRenderState
        .depth(false)
    ;
}

void nsm::UILayer::draw(const nsm::RenderInfo& renderInfo) {
    mRenderState.apply();

    nsm::RenderInfo info = renderInfo;
    info.camera = &mCamera;

    for (auto& drawable : mDrawables) {
        drawable->drawOpaque(info);
    }

    for (auto& drawable : mDrawables) {
        drawable->drawTranslucent(info);
    }
}

void nsm::UILayer::resize(const glm::u32vec2& size) {
    mCamera.setProjection(1.0f, -1.0f, this->getAspectRatio(), -this->getAspectRatio(), -1.0f, 1.0f);
}

f32 nsm::UILayer::getAspectRatio() const {
    glm::vec2 framebufferSize = Graphics::getFramebufferSize();
    return framebufferSize.x / framebufferSize.y;
}
