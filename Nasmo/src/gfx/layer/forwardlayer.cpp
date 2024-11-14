#include <nsm/gfx/layer/forwardlayer.h>

#include <nsm/entity/component/drawablecomponent.h>

nsm::ForwardLayer::ForwardLayer(const std::string& name)
    : Layer(name)
{
    mRenderState
        .depth(nsm::RenderState::DepthFunction::Less, true)
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
