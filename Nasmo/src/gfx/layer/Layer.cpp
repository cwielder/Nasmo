#include <nsm/gfx/layer/Layer.h>

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
