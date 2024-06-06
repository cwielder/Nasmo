#include <nsm/gfx/layer/modellayer.h>

#include <nsm/gfx/model.h>
#include <nsm/entity/component/modelcomponent.h>
#include <nsm/gfx/opengl.h>

#include <unordered_set>

nsm::ModelLayer::ModelLayer(const std::string& name)
    : Layer(name)
{
    mRenderState
        .depth(RenderState::DepthFunction::Less, true)
        .cull(RenderState::CullFace::Back, RenderState::CullDirection::CounterClockwise)
        .blend(false)
        .srgb(true)
    ;
}

void nsm::ModelLayer::draw(const RenderInfo& renderInfo) {
    mRenderState.apply();

    std::unordered_set<Model*> models; // TODO: Keep sets of models that use the same shader, so we can batch them together, and then sort by material to batch those too
    for (const auto& component : mDrawables) {
        auto modelComponent = dynamic_cast<ModelComponent*>(component);
        if (modelComponent) {
            models.insert(modelComponent->getModel());
        }
    }

    for (auto model : models) {
        model->drawOpaque(renderInfo);
    }

    for (auto model : models) {
        model->drawTranslucent(renderInfo);
    }
}
