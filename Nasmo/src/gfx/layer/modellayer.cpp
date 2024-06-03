#include <nsm/gfx/layer/modellayer.h>

#include <nsm/gfx/model.h>
#include <nsm/entity/component/modelcomponent.h>

#include <set>

void nsm::ModelLayer::draw(const RenderInfo& renderInfo) {
    std::set<Model*> models;

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
