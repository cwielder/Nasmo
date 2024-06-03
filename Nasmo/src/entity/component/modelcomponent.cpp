#include <nsm/entity/component/modelcomponent.h>

std::map<std::string, nsm::Model*> nsm::ModelComponent::sModels;

nsm::ModelComponent::ModelComponent(const std::string& path, const std::string& layer)
    : mModel(nullptr)
    , mInstanceData()
{
    auto it = sModels.find(path);
    if (it == sModels.end()) {
        sModels[path] = new Model(path);
    }
    mModel = sModels[path];

    mModel->addInstance(&mInstanceID);

    this->setTargetLayer(layer);
}

nsm::ModelComponent::~ModelComponent() {
    mModel->removeInstance(mInstanceID);

    if (mModel->getInstanceIDs().empty()) {
        auto it = sModels.find(mModel->getPath());
        delete (*it).second;
        sModels.erase(it);
    }
}
