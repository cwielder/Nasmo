#include <nsm/gfx/layer/skyboxlayer.h>

#include <nsm/entity/component/drawablecomponent.h>

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
