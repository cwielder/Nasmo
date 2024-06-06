#include <nsm/gfx/layer/layer.h>

#include <nsm/entity/component/drawablecomponent.h>

nsm::Layer::Layer(const std::string& name)
    : mDrawables()
    , mName(name)
    , mCamera(nullptr)
    , mRenderState()
{
    mRenderState
        .depth(RenderState::DepthFunction::LessEqual, true)
    ;
}

void nsm::Layer::draw(const nsm::RenderInfo& renderInfo) {
    mRenderState.apply();

    for (auto& drawable : mDrawables) {
        drawable->drawOpaque(renderInfo);
    }

    for (auto& drawable : mDrawables) {
        drawable->drawTranslucent(renderInfo);
    }
}
