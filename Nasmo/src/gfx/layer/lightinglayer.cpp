#include <nsm/gfx/layer/lightinglayer.h>
#include <nsm/entity/component/lightcomponent.h>

nsm::LightingLayer::LightingLayer(const std::string& name)
    : Layer(name)
{
    mRenderState
        .depth(false)
        .cull(RenderState::CullFace::Front, RenderState::CullDirection::CounterClockwise)
        .blend(RenderState::BlendFactor::One, RenderState::BlendFactor::One, RenderState::BlendEquation::Add)
    ;
}

void nsm::LightingLayer::draw(const RenderInfo& renderInfo) {
    for (auto& drawable : mDrawables) {
        drawable->drawOpaque(renderInfo);
        // lights don't use the tranlucent pass
    }
}
