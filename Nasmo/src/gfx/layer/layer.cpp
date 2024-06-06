#include <nsm/gfx/layer/layer.h>

#include <nsm/entity/component/drawablecomponent.h>

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
        drawable->drawOpaque(renderInfo);
    }

    for (auto& drawable : mDrawables) {
        drawable->drawTranslucent(renderInfo);
    }
}
