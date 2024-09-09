#include <nsm/gfx/layer/lightinglayer.h>
#include <nsm/entity/component/lightcomponent.h>

nsm::LightingLayer::LightingLayer(const std::string& name, const LightingLayer::Type type)
    : Layer(name)
{
    switch (type) {
        case Type::Point: {
            mRenderState
                .depth(false)
                .cull(RenderState::CullFace::Back, RenderState::CullDirection::CounterClockwise)
                .blend(RenderState::BlendFactor::One, RenderState::BlendFactor::One, RenderState::BlendEquation::Add)
            ;

            break;
        }

        case Type::Directional: {
            mRenderState
                .depth(false)
                .cull(false)
                .blend(RenderState::BlendFactor::One, RenderState::BlendFactor::One, RenderState::BlendEquation::Add)
            ;

            break;
        }
    }

}

void nsm::LightingLayer::draw(const RenderInfo& renderInfo) {
    mRenderState.apply();

    for (auto& drawable : mDrawables) {
        drawable->drawOpaque(renderInfo);
        // lights don't use the translucent pass
    }
}
