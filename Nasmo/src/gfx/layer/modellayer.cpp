#include <nsm/gfx/layer/modellayer.h>

#include <nsm/gfx/model.h>
#include <nsm/entity/component/modelcomponent.h>
#include <nsm/gfx/opengl.h>

#include <unordered_set>

nsm::ModelLayer::ModelLayer(const std::string& name)
    : Layer(name)
{
    mGraphicsContext
        .depth(GraphicsContext::DepthFunction::Less, true)
        .cull(GraphicsContext::CullFace::Back, GraphicsContext::CullDirection::CounterClockwise)
        .blend(false)
        .srgb(true)
    ;
}

void nsm::ModelLayer::draw(const RenderInfo& renderInfo) {
    mGraphicsContext.apply();

    std::unordered_set<Model*> models;
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
