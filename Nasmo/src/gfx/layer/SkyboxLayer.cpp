#include <nsm/gfx/layer/SkyboxLayer.h>

#include <nsm/entity/component/DrawableComponent.h>

nsm::SkyboxLayer::SkyboxLayer(const std::string& name)
    : Layer(name)
{
    mRenderState
        .depth(RenderState::DepthFunction::LessEqual, true)
    ;
}

void nsm::SkyboxLayer::draw(const RenderInfo& renderInfo) {
    mRenderState.apply();

    for (const auto& drawable : mDrawables) {
        drawable->drawOpaque(renderInfo);
    }
}
