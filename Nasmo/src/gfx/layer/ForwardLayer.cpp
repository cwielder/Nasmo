#include <nsm/gfx/layer/ForwardLayer.h>

#include <nsm/entity/component/DrawableComponent.h>

nsm::ForwardLayer::ForwardLayer(const std::string& name)
    : Layer(name)
{
    mRenderState
        .blend(RenderState::BlendFactor::SrcAlpha, RenderState::BlendFactor::OneMinusSrcAlpha, RenderState::BlendEquation::Add)
    ;
}

void nsm::ForwardLayer::draw(const nsm::RenderInfo& renderInfo) {
    mRenderState.depth(nsm::RenderState::DepthFunction::Less, true);
    mRenderState.apply();

    for (auto& drawable : mDrawables) {
        drawable->drawOpaque(renderInfo);
    }

    mRenderState.depth(nsm::RenderState::DepthFunction::Less, false);
    mRenderState.apply();

    for (auto& drawable : mDrawables) {
        drawable->drawTranslucent(renderInfo);
    }
}
