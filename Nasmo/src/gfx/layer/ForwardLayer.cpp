#include <nsm/gfx/layer/ForwardLayer.h>

#include <nsm/entity/component/DrawableComponent.h>

nsm::ForwardLayer::ForwardLayer(const std::string& name)
    : Layer(name)
{
    mRenderState
        .depth(nsm::RenderState::DepthFunction::Less, true)
        .blend(RenderState::BlendFactor::SrcAlpha, RenderState::BlendFactor::OneMinusSrcAlpha, RenderState::BlendEquation::Add)
    ;
}

void nsm::ForwardLayer::draw(const nsm::RenderInfo& renderInfo) {
    mRenderState.apply();

    for (auto& drawable : mDrawables) {
        drawable->drawOpaque(renderInfo);
    }

    for (auto& drawable : mDrawables) {
        drawable->drawTranslucent(renderInfo);
    }
}
